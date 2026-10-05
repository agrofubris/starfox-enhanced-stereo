# Leia SR (Simulated Reality) integration — implementation reference

How this port drives a Leia SR autostereoscopic panel: the architecture, the
exact per-frame flow, every failure path, and the traps that cost debugging
sessions. Written so the same approach can be lifted into another game or
engine.

Attribution chain: the approach follows oneup03's RT64 3D work
(<https://github.com/oneup03/rt64-3D>) and bo3b's SR-lib wrapper
(<https://github.com/bo3b/SR-lib>); the stereo fork implementation described
here is by agrofubris; kandowontu refined it in the upstream Star Fox Enhanced
port (optional loader DLL, `owned-weave` bridge, `SR PLATFORM` / DisplayXR
routes — see §11). All are credited in `CREDITS.md` and
`THIRD_PARTY_NOTICES.md`.

The SR SDK is proprietary and is **not** redistributed with this repository.
Everything here builds without it (the mode degrades to plain full SBS), and
the final executable delay-loads every SR/OpenCV DLL so machines without the
SR Platform never see a missing-DLL popup.

---

## 1. What the panel needs

A Leia SR panel is a lenticular display: the hardware interleaves two views
under the lens array, and the SR runtime (`SRPlatform` service + display
driver) computes the weave for the specific panel. The app does **not** do the
interleaving itself. The app hands the runtime a single texture containing a
complete, full-resolution side-by-side pair and asks it to "weave" that pair
into the panel's output texture.

Consequences that drive the whole design:

- The pair must be at **panel output resolution**: each half is one complete
  eye view at the size the panel expects. Half-resolution or squeezed eyes
  make the lens alignment drift.
- Input and output should be **B8G8R8A8** (the SDK's examples and pipeline use
  that byte order; an RGBA input can weave black).
- The weave runs on the app's **D3D12 command list**, so the app must own the
  D3D12 device and the output texture. In this port that means the SDL GPU
  backend must be D3D12.
- The runtime/display/service may be absent. Every step must degrade to
  presenting the raw side-by-side image.

## 2. Architecture at a glance

```
game frame (indexed 256-color + palette)
   -> GPU composition per eye (starfox::render::GpuComposite)
   -> stereo_eye_textures_[2]            (SDL_Texture, one per eye)
   -> packed pair texture                (SDL_Texture, BGRA32, 2W x H)
        present_leia_sr():
          SR-lib CreateSRInterfaceDX12(ID3D12Device, HWND)
          SDL present-weave bridge:
             bind app-owned BGRA target as RTV
             transition source -> PIXEL_SHADER_RESOURCE
             callback -> SRInterfaceDX12::Weave(command_list, ...)
          submit SDL command buffer
   -> stereo_leia_texture_ (SDL_Texture, BGRA32, W x H)   [woven by the SDK]
   -> SDL_RenderTexture + SDL_RenderPresent                [normal SDL present]
```

Failure at any arrow falls back to presenting the packed side-by-side pair
directly (the same image a passive 3D display or SBS viewer would show).

## 3. Reference implementation map

| File | Role |
|---|---|
| `include/starfox/render/stereo_output.hpp` | `StereoOutput::leia_sr`; its layout is identical to full SBS (covered by `tests/stereo_output_tests.cpp`). |
| `src/app/leia_sr_host.hpp` | Thin host around the SR-lib DX12 weaver: lazy create, unavailable latch, per-frame `weave()`, exception-safe `release()`/`disable()`. Compiles to a no-op stub when `STARFOX_LEIASR` is not defined. |
| `include/starfox/render/sdl_d3d12_bridge.h` | Versioned private ABIs installed on the pinned SDL D3D12 renderer: device/buffer/fence bridge, texture copy bridge, compute bridge, present hooks, and the `STARFOX_SDL_D3D12_PRESENT_WEAVE` bridge used by Leia SR. |
| `src/render/sdl_d3d12_bridge.inc` | The bridge implementations, compiled *inside* SDL's D3D12 backend (`Starfox_D3D12_PresentWeave` and friends). |
| `cmake/sdl3-d3d12-interop.cmake` | Idempotently injects the include and the renderer properties into the fetched SDL source; guarded, anchor-checked, fails configure if the pinned source changed. |
| `CMakeLists.txt` | `STARFOX_ENABLE_LEIASR` (default ON), `STARFOX_SRLIB_DIR` (default `third_party/SR-lib`), links `SRLib::SR`, defines `STARFOX_LEIASR`, calls `srlib_apply_delayload(starfox_pc)`. |
| `src/app/starfox_pc.cpp` | `present_leia_sr()` and the Leia branch of `present_stereo()`; backend switching; device-loss recovery; diagnostics. |
| `third_party/SR-lib` | Local SDK checkout, git-ignored (`/third_party/SR-lib/`). Not committed. |
| `leia-debug.bat`, `leia-perf.bat` | Tester launchers (kept untracked in the repo, shipped inside the release zip). |

## 4. The present-weave bridge (the core trick)

The SR runtime must record its weave on the same D3D12 command list that will
present, with the output texture bound as a render target. SDL's GPU API does
not expose its command list or its resource transitions, so this port pins
SDL 3.4.14, injects a small private ABI into `SDL_gpu_d3d12.c`, and reads it
back through renderer properties. This is a private ABI, not an SDL public
API.

ABI (`sdl_d3d12_bridge.h`):

```c
#define STARFOX_SDL_D3D12_PRESENT_WEAVE "starfox.gpu.d3d12.present-weave.v1"
typedef bool (*StarfoxD3D12PresentWeaveCallback)(void *user, void *command_list,
    void *native_source, uint32_t width, uint32_t height, uint32_t output_format);
typedef struct StarfoxSdlD3D12PresentWeaveBridgeV1 {
    uint32_t version;
    bool (*weave)(void *command, void *source_texture, void *swapchain_texture,
        uint32_t width, uint32_t height,
        StarfoxD3D12PresentWeaveCallback callback, void *user);
} StarfoxSdlD3D12PresentWeaveBridgeV1;
```

Contract implemented in `Starfox_D3D12_PresentWeave`
(`src/render/sdl_d3d12_bridge.inc`):

1. Validate: single-mip, single-sample 2D SDL textures, distinct resources.
2. Transition the source from its default state to
   `D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE`; transition the target to
   `D3D12_RESOURCE_STATE_RENDER_TARGET`.
3. Bind the target's RTV, set viewport/scissor to the weave extent, bind the
   renderer's descriptor heaps, and hand the callback the command list, the
   source's **native `ID3D12Resource*`**, the extent, and the target RTV's
   **DXGI format** (`SDLToD3D12_TextureFormat[...]`).
4. The callback runs the SDK weave. It must not submit or close the command
   list and must not throw across the C boundary.
5. SDL unbinds the render target, transitions both textures back to their
   default states, rebinds descriptor heaps, and resets its cached pipeline
   bindings (`currentGraphicsPipeline`/`currentComputePipeline`) because the
   native code changed command-list state behind SDL's back.

Two deliberate restrictions:

- **The target must be an ordinary app-owned SDL texture, never a freshly
  acquired swapchain texture.** An acquired swapchain is already a render
  target with its own transition bookkeeping, so it skips the contract and
  is not a supported path (see gotcha #1).
- **The callback gets the source's native resource, not the SDL texture.** The
  host calls `SetInputTexture` on it every frame (see gotcha #6).

Injection (`cmake/sdl3-d3d12-interop.cmake`): reads
`${SOURCE_DIR}/src/gpu/d3d12/SDL_gpu_d3d12.c`, inserts
`#include "sdl_d3d12_bridge.inc"` before `D3D12_CreateDevice`, and appends
`SDL_SetPointerProperty(renderer->props, STARFOX_SDL_D3D12_PRESENT_WEAVE, ...)`
after device creation. It checks for existing markers first, so reconfiguring
is a no-op; if the anchors or partial markers don't match, configure fails
loudly instead of producing a half-patched backend. The same script installs
the swapchain present hooks used by the DLSS path.

The app reads the bridge from the device:

```cpp
auto* gpu = static_cast<SDL_GPUDevice*>(effect_device());
const auto* bridge = static_cast<const StarfoxSdlD3D12PresentWeaveBridgeV1*>(
    SDL_GetPointerProperty(SDL_GetGPUDeviceProperties(gpu),
        STARFOX_SDL_D3D12_PRESENT_WEAVE, nullptr));
```

## 5. The host class (`src/app/leia_sr_host.hpp`)

Thin, exception-safe wrapper around `SimulatedReality::SRInterfaceDX12`:

- `ensure(device, hwnd)` — lazy `CreateSRInterfaceDX12`. On any failure
  (`FAILED(hr)`, null, or a C++ throw from the runtime) it sets
  `unavailable_ = true` and returns false. The latch is deliberate: the probe
  is never retried per frame. `release()` clears the latch for a new device.
- On success it calls `SetShaderSRGBConversion(false, false)` — see gotcha
  #5.
- `weave(command_list, native_source, width, height, output_format)`:
  `SetInputTexture(native_source)` **every frame**, `SetOutputFormat(...)`
  only when the format changes, then `Weave(command_list, viewport, scissor)`.
  Returns false on failure instead of throwing.
- `release()` — `Delete()` wrapped in `try/catch` (a lost device makes the
  SDK throw), then drops the pointer. The interface itself is never
  `delete`d: `Delete()` destroys the weaver and then the context.
- `disable()` — same teardown but latches `unavailable_`, used after a failed
  weave so the app stops retrying a broken runtime on this device.

Without `STARFOX_LEIASR` the class is a stub returning false/true
unavailable, so the whole app compiles unchanged without the SDK.

## 6. App integration (`src/app/starfox_pc.cpp`)

### Mode switching and backend selection

- The Options row presents `StereoOutput::leia_sr` as `LEIA SR PANEL`.
- Once per frame the loop calls
  `window.set_leia_sr_requested(game.stereo_output() == leia_sr)`. On change
  it releases the weaver and, when the GPU renderer is active, recreates the
  renderer so SDL picks the right backend. Crossing into or out of the mode
  therefore costs one renderer recreation.
- `SDL_HINT_GPU_DRIVER` is set to `direct3d12` with `SDL_HINT_DEFAULT`
  priority when the mode is requested (explicit env/test overrides still
  win). Without D3D12 the mode silently falls back to side-by-side.
- `release_renderer_resources()` calls `leia_sr_.release()` **first**, before
  any texture or the device is destroyed: the weaver may still hold views
  over the packed texture.

### Per-frame flow (`present_stereo` -> `present_leia_sr`)

1. Compute the panel extent from `SDL_GetRenderOutputSize`, force it **even**
   (`&= ~1`) so each side-by-side half is integral.
2. Pack the pair: render each eye into its half of `stereo_packed_texture_`
   (BGRA32, `2W x H`). The game's display aspect is fitted inside each half
   with black bars rather than shrinking content (lens alignment depends on
   full-resolution halves).
3. Create/recreate `stereo_leia_texture_` (BGRA32, `W x H`, render-target
   access) when the panel extent changes; mark `leia_size_settled_ = false`.
4. `present_leia_sr()`:
   1. `STARFOX_DISABLE_LEIA_WEAVE` set? decline (tester escape hatch).
   2. Host already latched unavailable? decline.
   3. GPU driver must be `direct3d12`; otherwise decline.
   4. Read the native `ID3D12Device*` from the `STARFOX_SDL_D3D12_DEVICE`
      renderer property and the `HWND` from
      `SDL_PROP_WINDOW_WIN32_HWND_POINTER`.
   5. `leia_sr_.ensure(device, hwnd)`; decline on failure.
   6. Fetch the weave bridge; if missing, `disable()` and decline.
   7. Resolve both textures to `SDL_GPUTexture*` via
      `SDL_GetPointerProperty(SDL_GetTextureProperties(t),
      SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, ...)`.
   8. If `!leia_size_settled_`: set it and decline for exactly one frame so
      SDL and the panel settle after a resize (gotcha #4).
   9. `SDL_FlushRenderer` (the weave samples the composed pack on the same
      D3D12 queue through a separate command buffer), acquire an SDL GPU
      command buffer, call `bridge->weave(...)` with a C callback that
      forwards to `LeiaSrHost::weave`.
   10. On weave failure: cancel the command buffer, `disable()`, decline.
   11. Submit and return true.
5. On success, present the woven texture 1:1: disable logical presentation,
   clear, `SDL_RenderTexture(stereo_leia_texture_)`, `SDL_RenderPresent`.
6. On any decline, fall through to the plain side-by-side presenter, which
   presents `stereo_packed_texture_` (same image, no weaving).

### Device loss

The SR panel's display-mode switch can invalidate the swapchain under the
device. `gpu_device_lost()` reads `ID3D12Device::GetDeviceRemovedReason()`
through the native device property; `note_device_loss()` marks it and
`recover_device_loss()` recreates the renderer instead of exiting. A lost
device costs one frame, not the session.

## 7. Gotchas (each one cost a debugging session)

1. **Never weave into the raw SDL swapchain.** On an SR panel the display-mode
   switch invalidates the backbuffer: black image, continuous RAM growth, then
   device removed (`0x887A0005`) surfacing at `SDL_UpdateTexture`. Weave into
   an app-owned texture and let SDL present it.
2. **Input and output must be B8G8R8A8.** An RGBA input can weave black. The
   packed pair is created BGRA32 only in Leia mode; the other stereo modes
   keep RGBA.
3. **Force even output extents.** An odd width makes each side-by-side half
   fractional.
4. **Skip one frame after a size/format change.** SDL and the panel need a
   frame to settle before weaving into the new target.
5. **`SetShaderSRGBConversion(false, false)`** when both the intermediate and
   the panel surface are UNORM and the compose already encoded the game's
   gamma; otherwise the panel gets double gamma.
6. **Re-set the input texture every frame.** The SDK reads size and format
   from the resource desc, and a cached view goes stale across a resize.
7. **Delay-load every SR/OpenCV DLL** (`srlib_apply_delayload(starfox_pc)`),
   or machines without the SR Platform get a missing-DLL popup before
   `main()`. Verify with `dumpbin /imports starfox_pc.exe`: SR DLLs must
   appear only under "delay load imports".
8. **Latch the unavailable state.** A missing runtime/display/service is
   normal (`CreateSRInterfaceDX12` -> `E_NOINTERFACE`); probe once, log once,
   present side-by-side. Never retry per frame.
9. **A failed weave must not blank the window.** Cancel the command buffer,
   disable the host for this device, keep presenting the fallback.
10. **Do not let `Delete()` exceptions escape during teardown.** A device lost
    under the weaver makes the SDK throw; `release()` swallows it.
11. **Per-eye reflection offsets must follow the live separation and
    convergence.** Hardcoding them to the default values broke reflective
    surfaces at custom stereo settings (fixed in `239bd32`).
12. **Do not cap separation at typical stereo defaults.** The SR tester's
    reference presets use separation 34.00 with convergence 208, and the
    original UI ceiling of 12.80 blocked them (fixed in `239bd32`: the range
    now spans 0.25..655.35, 0.25 steps below 12.80 and 1.00 above). SR users
    need the game's full per-eye parallax budget.
13. **The weave needs complete per-eye views at panel resolution.** Fit the
    game image inside each half with black bars; do not shrink or squeeze it.

## 8. Build integration

```cmake
option(STARFOX_ENABLE_LEIASR "Link the optional Leia SR D3D12 weaver" ON)
set(STARFOX_SRLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SR-lib" CACHE PATH ...)

if(WIN32 AND NOT WINDOWS_STORE AND STARFOX_ENABLE_LEIASR
   AND EXISTS "${STARFOX_SRLIB_DIR}/CMakeLists.txt")
    add_subdirectory("${STARFOX_SRLIB_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/srlib")
    if(SRLIB_FOUND) set(STARFOX_LEIASR_ENABLED TRUE) endif()
endif()
```

When enabled: link `SRLib::SR`, define `STARFOX_LEIASR` on the runtime target,
call `srlib_apply_delayload(starfox_pc)`. Without an SR-lib checkout the
build logs `Leia SR: no SR-lib at ...; building stub fallback` and the mode
falls back to full SBS at runtime. `third_party/SR-lib/` is git-ignored.

The D3D12 bridge injection is invoked from `CMakeLists.txt` alongside the DXR
setup (`STARFOX_DXR_ENABLED` on Windows, or the Linux runtime path), on the
pinned SDL fetched as the official 3.4.14 tarball with a SHA-256. A port that
cannot pin/patch SDL this way needs an equivalent hook in whatever owns its
command list.

## 9. Diagnostics

| Env var / launcher | Effect |
|---|---|
| `STARFOX_TRACE_GPU=1` | Flow log: `leia-sr: weaver initialized`, `leia-sr: wove WxH`, `stereo presented: leia-sr`, and the exact fallback stage on failure. |
| `STARFOX_CAPTURE_LEIA_PATH=<file.bmp>` | Readback of the woven panel texture after a successful weave. Black file = the weave produced nothing; good file + black window = a presentation problem instead. |
| `STARFOX_DISABLE_LEIA_WEAVE=1` | Force the side-by-side fallback without touching the weaver (isolates the SDK from the rest of the pipeline). |
| `STARFOX_TRACE_GPU_PASS_COST=1` | Per-stage timings (compose, setup, effects, draw) for performance triage. |
| `leia-debug.bat` | Double-click tester: runs the game with the trace and capture enabled, writes `leia-log.txt` + `leia.bmp`. |
| `leia-perf.bat` | Double-click tester: trace + pass costs only, writes `leia-perf.txt`. |

Expected log on a machine **without** SR hardware:
`leia-sr: CreateSRInterfaceDX12 failed (hr=0x80004002); presenting
side-by-side`, followed by the normal full-SBS presentation.

## 10. Verification status

- Implemented in `5d0b789` (Leia SR output + top-and-bottom modes).
- Weave-into-swapchain failure diagnosed and fixed in `824ed0f` (app-owned
  texture, state transitions, even extents, settle frame, wrapped SDK
  create/delete); stale SDL error text in the fallback trace removed in
  `4177b21`; device-loss survival added in `f994ae5`.
- Credits added in `fb2f2da`; label clarified in `d540f72`.
- SR-panel tester feedback raised the separation ceiling from 12.80 to 655.35
  (`239bd32`, confirmed fixed 2026-10-03): the reference presets use
  separation 34.00 with convergence 208, which the old ceiling blocked.
- Confirmed working on a physical SR panel (windowed and fullscreen) by an
  SR-panel tester on 2026-10-03, on the `v0.0.8-stereo2` pre-release.
- The upstream port's refinement (optional loader DLL + `owned-weave`) was
  confirmed working on an SR panel by the same tester on 2026-10-05 with
  full-resolution per eye, a working 3D reticle and 120 fps at 120 Hz at 3x
  render scale — see §11.
- On a machine without SR hardware the fallback path is exercised every run;
  `tests/stereo_output_tests.cpp` asserts the Leia layout equals full SBS.

## 11. Official-port refinement (kandowontu, October 2026)

The upstream Star Fox Enhanced port integrated the same direct-SDK route into
its Windows x64 builds and refined the packaging and diagnostics. Its
`OUTPUT: SR PLATFORM` mode is the approach described above; a separate
`DISPLAYXR LEIA` toggle selects an OpenXR route (see below). The changes are
worth copying when porting.

- **SDK binding in an optional loader DLL.** Instead of linking SR-lib into
  the executable, a separate `starfox_leia_sr.dll` exposes a C ABI
  (`starfox_leia_sr_get_api`, `starfox_leia_sr_status`) and is loaded
  optionally at runtime. The DLL is built against Leia SR SDK 1.34.10
  headers/import libraries from bo3b/SR-lib's `api_expansion` commit
  `0c80cc0` and uses the official SDK API (`SR::SRContext`,
  `SR::CreateDX12Weaver`, `SR::SwitchableLensHint`) rather than the
  `SRInterfaceDX12` wrapper. It delay-loads only
  `SimulatedRealityCore/Displays/DirectX.dll` — no OpenCV, no face trackers.
  The game executable therefore keeps no SR imports at all and the SDK module
  stays replaceable.
- **Its own SDL weave bridge.** `starfox.gpu.d3d12.owned-weave.v1` replaces
  the `present-weave.v1` ABI used here; the app-owned-texture contract is the
  same.
- **Diagnostics as a product feature.** `starfox_pc --leia-sr-check
  [DISPLAYXR_RUNTIME_DIRECTORY]`, a standalone `starfox_leia_sr_check.exe`,
  `Diagnose-Leia-SR.cmd` and `startup.log`, with explicit status strings for
  clone mode, no active display, window off panel, pending teardown, etc.
- **`DISPLAYXR LEIA` is a separate route.** DisplayXR
  (<https://displayxr.org>) is an open-source OpenXR runtime for 3D displays
  with a Leia SR weaver plug-in. Selecting it routes presentation through
  that runtime instead of the direct SDK; it requires the DisplayXR runtime
  and plug-in to be installed. With only the SR Platform installed it reports
  `UNAVAILABLE` and leaves no 3D output (no automatic fallback to
  `SR PLATFORM`), so testers should leave it OFF. A port could improve this
  by hiding/graying the option or falling back when the runtime is missing.
- **Verified performance (SR-panel tester, 2026-10-05):** full resolution per
  eye (not half-SBS upconverted), the 3D reticle performs fine, and the game
  holds 120 fps at 120 Hz at 3x render scale with ray tracing (occasional 110
  at 4x). Only volumetric fog and motion blur were reported heavy, both
  unrelated to 3D. This supersedes the earlier reticle FPS regression report
  against the pre-refinement build.

## 12. Porting checklist

1. **Own the command list.** Find the layer that records/presents the final
   frame and expose a callback that runs with an app-owned output texture
   bound as render target, with source/target transitions handled for you.
   In SDL that meant a small private ABI injected into the pinned backend.
2. **Keep the SDK behind a host class** with lazy init, an unavailable latch,
   a per-frame `weave()`, and exception-safe teardown that runs before the
   device is released.
3. **Make the mode a first-class output option** whose layout is full SBS;
   test that its geometry matches the existing full-SBS path.
4. **Prefer the D3D12 backend when the mode is selected** and recreate the
   renderer on mode changes.
5. **Pack at panel resolution, BGRA, even extents**, and present the woven
   texture through the normal present path.
6. **Fall back at every step** (no runtime, no display, no D3D12, missing
   bridge, failed weave) to the raw side-by-side image.
7. **Add diagnostics and tester launchers** before shipping; a black weave
   and a black window are different failures.
8. **Delay-load** the SDK DLLs and keep the SDK out of version control.
   Consider the optional loader-DLL variant from §11 so the game executable
   carries no SDK imports at all.
9. **Credit the chain:** oneup03 (RT64 3D approach), bo3b (SR-lib wrapper and
   build guidance), agrofubris (stereo fork implementation), kandowontu
   (official-port refinement).
