# ScaleFX integration (unreleased)

SCALEFX is selectable under 2D OPTIONS, with saved-setting and save-state
validation for value 5. Existing filter values are unchanged. The proper name
is retained in every menu language.

The five standard ScaleFX passes are implemented in C++ and portable GPU
compute. Both follow libretro/slang-shaders commit
`b61e1ee4fc9e2119ec933461a0bfad024dd2950a`, standard `scalefx.slangp`,
with threshold .5 and AA/corner filtering enabled. Source attribution and MIT
permission are retained in both ports and THIRD_PARTY_NOTICES.md.

The algorithm reconstructs at 3x, selecting original colours. Other render
scales sample that reconstruction; native 1x uses the existing alpha-weighted
area resolve. Textured polygons are eligible artwork, but final writes retain
their original coverage. Solid geometry remains excluded from the 2D filter.

## GPU and fallback

`GpuScaleFx` records on the existing SDL GPU device/command buffer. Persistent
scratch buffers and an optional texture adapter keep the complete chain on
device, without readback, CPU wait or extra device. The SDL effects compositor
extracts its artwork texture and resolves the result with existing layer masks.

C++ uses reusable per-pass buffers and the shared row-worker pool. It covers
software rendering, D3D11's declined ScaleFX pass and standalone overlays.
It is the real five-pass algorithm, not an EDGE substitution.

`tools/generate_scalefx.py` generates SPIR-V and Metal sources; CMake rejects
stale generated headers. Metal generation is not physical-device verification.

## Evidence

- Windows Vulkan: 60 CPU/GPU pixel-exact cases through buffer and texture inputs.
  Shapes: 17x11, 1x1, 31x23, 7x3, repeated 17x11. Patterns include diagonals,
  checkerboards, coloured noise and alpha holes. Allocation growth/reuse,
  source-colour preservation and centre-pixel invariants are covered.
- Full CPU/GPU masked composition matches at 1x/2x/3x/4x, including exclusion
  of untextured geometry. All 108 existing GPU composition cases still pass.
- Windows pixel-filter tests include ScaleFX at 1x/2x/3x/4x/6x/10x.
- Windows settings tests round-trip every filter, including ScaleFX.
- Matching EX TITLEMAP captures: 300 preroll ticks, 180 frames, 16:9, 4x.
  Runtime reports GPU-resident native raster; saved user settings are untouched.

![Filter Off](../tmp/scalefx-title-OFF.bmp)
![ScaleFX](../tmp/scalefx-title-SCALEFX.bmp)

- Independent upstream GLSL: `tools/check_scalefx_reference.py` executes all
  five original passes in an OpenGL 4.5 EGL context (Mesa llvmpipe), adapting
  only shader interfaces/stage markers. It uses the preset's float pass 0/1
  targets, normalized-byte pass 2/3 targets, nearest/clamp sampling, threshold
  .5 and AA/corner filtering on. All 24 distinct exported fixtures match the
  C++ reconstruction pixel-for-pixel, including noise and alpha holes.
  The original 60-case Vulkan check also passes after adding fixture export.
  Reproduce after installing moderngl/glcontext in an isolated Python environment:

  ```text
  starfox_scalefx_check tmp/scalefx-independent-fixtures
  python tools/check_scalefx_reference.py tmp/scalefx-reference/edge-smoothing/scalefx/shaders tmp/scalefx-independent-fixtures
  ```

  The reference runner initially supplied pass 1's threshold to pass 3's
  corner toggle. Intermediate comparisons isolated that harness error; fixing
  the parameter yielded exact agreement without changing production math.

Broader runtime/overlay captures, physical Metal/console tests and performance
measurements remain useful verification. This feature does not imply that the
separate full GPU migration or VR objective is complete.
