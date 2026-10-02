# Active delivery scope

For the current requirement-by-requirement status, use **GOAL-ACCEPTANCE.md**.
The entries below are chronological history; newer GPU migration evidence in
GPU-MIGRATION-STATUS.md supersedes the old pending-import notes.

### D3D12 model pipeline (September 13)

Remaining generated shader families now include DXIL; model-related consumers
select it. D3D12 and Vulkan projection/BSP tests pass. D3D12 mixed models pass
24 models / 288 images per cartridge. Explicit-backend native gameplay is now
enabled; Original/EX 12-frame LEVEL1_1 comparisons pass exactly with resident
geometry, raster, composition, effects and hardware shadows. Verified captures/
logs: `tmp/d3d12-native-verified-original` and `tmp/d3d12-native-verified-ex`.

Test harness now preserves explicit backend requests in DefaultPipeline mode
and requires resident hardware shadows for D3D12 ray tests. The earlier
`d3d12-native-full-*` captures were Vulkan and must not be cited as D3D12 proof.
Default remains Vulkan; broader D3D12 coverage, Vulkan native DXR import and
remaining goal/headset acceptance are still open.

### D3D12 composition and scanlines (September 13)

Added DXIL to raster, binning, composition and portable shadow shaders and
their selectors. Explicit D3D12: 168 exact raster cases + 32 queued submissions,
full 108-base-case composition suite, and compute-shadow regression pass.
Vulkan full composition regression passes too. Windows build and shader checks
pass. Other model/BSP/projection producers remain missing on D3D12; keep native
game pipeline gating and Vulkan default until that work is verified.

### D3D12 ScaleFX (September 13)

Added offline DXIL for six ScaleFX stages and device-based selection. D3D12 and
Vulkan each pass sixty CPU/GPU cases plus effects integration at scales 1..4.
Live 2x Original LEVEL1_1 with ScaleFX and resident DXR succeeds; evidence is
`tmp/sdl-dxr-scalefx-live.bmp` (inspected) and `.log`. Windows build and shader
freshness check pass. Full default-backend migration is still incomplete.

### Live desktop resident shadows (September 13)

Connected SdlDxrShadows to Window/game-loop mono/stereo submission, selected
output routing, explicit fallback ownership and renderer teardown/reset.
Unsupported backends decline quickly; existing diagnostics keep downloads.
Original LEVEL1_1 at tick 1000 passes twelve-frame live D3D12 mono/Half SBS/
Full SBS runs, direct GPU presentation and both resident stereo eyes. Evidence:
`tmp/sdl-dxr-live-{0,1,2}.log` and corresponding BMPs (mono inspected).
Vulkan default exact native raster regression passes at the same checkpoint.

Default is still Vulkan because other native D3D12 shaders remain incomplete.
Its DXR readback remains pending native Vulkan/SDL import. This is a working
explicit-backend runtime connection, not completion of the default migration.

### Resident DXR owner (September 13)

SdlDxrShadows now owns producer/import/SDL destination and provides render,
fallback readback, output invalidation and borrowed-device release. Hardware
diagnostics exercise resize, explicit release, invalid-camera failure and
recovery with exact final blends. A real intermittent stale-import bug was
fixed: COM wrapper addresses can be reused on resize, so cached imports also
track allocation size. Five fresh runs / 60 frames passed after that fix.

Windows runtime builds, but Window/game-loop routing still needs integration.
Do not claim default runtime readback removed. Vulkan SDL bridge and all-model
D3D12 shader coverage remain distinct unfinished requirements.

### Packed-mask effects consumer (September 13)

Completed the next bridge component: resident packed-byte masks with validated
four-byte row stride now feed SDL effects without CPU unpacking. Offline effects
generation includes DXIL, and the SDL effects pipeline can select it. Twelve
animated/resized native DXR copies plus final layer-aware, vertically-offset
effects blends compare exactly with the existing uploaded-mask path. Invalid
strides are rejected. Vulkan portable-shadow regression also passes (the known
two 4x CPU edge differences remain). Shader freshness check passes.

Application wiring is not done: Window owns only PortableShadows buffers today,
and its CPU fallback lookup cannot accept a new owner's buffer yet. Mono/stereo
submission, reuse/release, fallback and device reset must be integrated together.
SDL Vulkan native import is still separate pending work. Do not enable runtime
DXR resident submission merely because this component diagnostic passes.

### Native SDL/DXR bridge foundation (September 13)

Added a private version-1 native D3D12 bridge to the pinned, locally built SDL
backend. It exposes the borrowed device and a checked GPU buffer-copy callback
through device properties; no private SDL structs cross the application ABI.
Copies transition the external resource COMMON -> COPY_SOURCE -> COMMON and
use SDL's destination cycling, barriers and lifetime tracking. The caller must
finish the producer and retain the source through consumer completion.

`starfox_sdl_d3d12_interop_check` passes twelve animated/resized real DXR masks
(133,400,129 pixel widths), including padded rows. Native shared handles are
opened on SDL's device and copied to SDL storage buffers. Diagnostic-only
downloads compare exactly with DXR; null/zero/oversized copies are rejected.
Windows game builds with the bridge. Configuration's guarded source insertion
is idempotent; the original Git patch attempt silently skipped the nested
dependency and was replaced, not accepted as evidence. Current build explicitly
uses workspace DXC at `tmp/portable-gpu-tools/dxc/bin/x64/dxc.exe`.

Not yet wired to desktop presentation: DXR emits packed bytes, while current
resident effects expect one uint32 per pixel. Add packed resident input support
or GPU expansion, then connect mono/stereo and verify lifetime/device fallback.
The runtime still uses readback today. Vulkan-backed SDL also needs a bridge;
PC VR's existing native Vulkan interop is separate. No FPS gain is claimed.

### EX city menu surround (September 13)

Native PGBG 9 selects `citycity`/`citypal` in GSTRATS2.ASM. Its captured
atlas matches the existing city landscape policy. Connected it to origin
248, unique-right skyline handling and the separately drawn planet patch.
Added a native menu fixture assertion. Front, rear and right GPU captures
passed and were inspected under `tmp/vr-ex-menu-city9-{front,rear,right}`:
the skyline surrounds the view and the planet group is visible at the right,
absent in the rear capture. These samples are not a complete angular sweep.
36/37 numbered menu choices plus blackout now have classified surrounds;
planet-face wallpaper 18 remains outstanding.

The complete VR suite passed 15/15 (28.91s) after the orbital shader change,
before this city classification. The broader migration remains incomplete.

### Orbital surface continuation (September 13)

Replaced the stretched/repeated orbital lower surface with a GPU-generated,
world-locked volumetric cloud field using colours sampled from the native
planet strip. The authored limb is retained with a short smooth blend below
it. Integer-hashed 3D noise avoids both flattened-XZ streaks and floating-point
hash seams. This is an artistic continuation, not newly recovered source art.
Normal, thin and entry orbital packets select the shared shader policy.

Packet regression passed. Native menu 25 front and 21 rear Vulkan captures
passed and were visually inspected in `tmp/vr-planet-continuation-seamless`
and `tmp/vr-planet-continuation-green`. Quest APK rebuilt successfully; not
installed. Headset visual acceptance and GPU cost remain unverified. Earlier
notes about stretched orbital caps below describe the superseded version.

### Remaining simple EX menu surrounds (September 13)

Captured and inspected native atlases for 19,20,21,22,23,25,26,27,28,29,30,31,
34,35,36,99 through the deterministic native menu route. Connected 19/27/28/31
to the existing single-planet plus star-only surround, 21/25/35 to orbital
lower hemispheres (25 retains its separate small planet), and the other
verified landscapes/patterns to their appropriate horizon/repetition policies.
Choice 20's low horizon now includes sky rows above its source viewport, and
99 selects black surroundings instead of the prior brown clear.

Rear captures under tmp/vr-ex-menu-surround-choice-N passed and were inspected
for the fifteen numbered choices. The generic nonblank test initially rejected
99's correct black rear; replaced this exception with an explicit zero-lit-pixel
assertion for native blackout rear/side views. It passes in
tmp/vr-ex-menu-blackout-rear-asserted. Added classification assertions; fresh
front captures pass in tmp/vr-ex-menu-planet19-front and
tmp/vr-ex-menu-orbital25-front. Original/EX input tests pass 2/2 (14.47s).

Classification covers 35/37 numbered choices plus blackout. Planet-filled
wallpapers 9 and 18 still need dedicated handling. Orbital surface stretching
is visible in the new captures (especially farther below the horizon), so
these samples establish coverage, not finished visual fidelity. All-headset,
all-level acceptance and the desktop resident DXR connection remain open.

### Deterministic EX menu choice coverage (September 13)

`TITLEMAP:menu-choice-N@direction` now selects 0..36 or 99 through the native
page-two Right handler. It seeds only page/cursor state, cycles the cartridge's
own PGBG selection, and lets native code reload graphics/palettes; it does not
substitute atlas data. Bounded cycling and final selection are asserted.

Captured/inspected authored atlases for 0,1,2,3,6,7,8,9,10,12,13,15,17,18.
Added surround classification for 0,1,2,3,6,7,8,10,12,13,15,17, with independent
fixture assertions of horizon and unique-left/right policy. Rear captures
passed and were inspected in tmp/vr-ex-menu-fixed-N for those twelve choices.
The planet-filled atlases 9 and 18 are deliberately still outstanding.

Low-origin (greater than 288) landscape atlases now sample sky latitude per
fragment rather than extruding their first visible cloud/smoke row upward.
Other landscape sky sampling is unchanged. Regenerated shaders and packet
checks pass; inspected tmp/vr-ex-menu-volcano-sky-fixed/live-scene-left.bmp
removes the old vertical smoke streak. The twin-moon front capture in
tmp/vr-ex-menu-moons-front-fixed retains its unique pair, absent in its rear
sample. Original/EX input tests pass 2/2 (16.66s), packet test passes (0.46s).

Twenty menu choices now have capture-backed handling. Remaining numbered
choices: 9,18,19,20,21,22,23,25,26,27,28,29,30,31,34,35,36; blackout 99 also
needs verification. Full physical-headset and all-background acceptance remain
open, as does the desktop SDL DXR resident-output connection.

### Further native menu variants and terminal ground (September 13)

Native delayed-Start captures identified five additional PGBG choices:
14 snow (delay 60), 16 red mountains (180), 11 night city (300), 24 abstract
pattern (420), and 4 blue-sky landscape (600; also 900). Corrected their
surround classification: landscapes retain their authored horizons, choice 4
keeps its unique sky object, and 24 repeats as a sphere without a false ground.
Rear captures passed and were inspected under tmp/vr-ex-menu-surround-{60,
180,300,420,600}. Original/EX input tests pass 2/2 (14.07s).

The city capture exposed vertical atlas wrapping: the lower ground sampled
sky after passing row 511. Landscape packets now flag vertical clamping in
the shared GPU tile shader; ordinary star/pattern spheres still repeat. Shader
regeneration/check and packet regression pass. The native city fixture with
@terminal-ground-check@rear passes both-eye pixel assertions against its
actual final atlas row. Inspected tmp/vr-ex-menu-city-floor-fixed/live-scene-left.bmp
shows black below the city instead of the prior blue-sky band.

Read EX source directly from the sparse checkout's Git objects:
SFES/GSTRATS2.ASM RANDOMIZEBG/bgtime defines choices 0..36; CONTINUE.ASM also
allows 99 (blackout). Eight choices now have capture-backed classification;
the remaining choices are not yet verified. This is not all-menu completion.

### Native EX menu background variants (September 13)

Added bounded `TITLEMAP:menu-N@direction` captures that delay Start by N
logic ticks, exercising native RANDOMIZEBG rather than forcing tile data.
The runtime trace now records PGBG. This exposed PGBG=5 (240-tick delay):
a desert Mode-2 atlas previously appeared as a flat panel against cyan clear.
Scene classification now reads PGBG without mutating the bus: 5 uses its
verified row-432 horizon (atlas origin 320) and unique monument half; 32 keeps
the existing moon layout; 33 uses the complete night-ocean panorama rather
than the moon's unique-half policy. Unknown choices are not blindly classified
as the initial moon scene. Other PGBG choices still require verification.

Fresh offscreen Vulkan captures passed and were inspected:
`tmp/vr-ex-menu-desert-{front,rear}-fixed/live-scene-left.bmp`,
`tmp/vr-ex-menu-ocean-rear-fixed/live-scene-left.bmp`, and
`tmp/vr-ex-menu-moon-rear-recheck/live-scene-left.bmp`. Desert now fills the
front/rear with sky and ground and does not duplicate the monument behind;
ocean and initial moon retain their distinct surroundings. Fixture asserts
the three choices' classification/origin/unique-half policy. Original/EX VR
input regressions pass 2/2 (15.17s). No headset acceptance is implied.
Quest APK rebuilt successfully in 50s with these changes. Not installed or
published; the user's app has not been interrupted.

### September 13 full regression refresh

Rebuilt both desktop and VR trees successfully. Current desktop CTest passes
54/54 in 342.18s; VR passes 15/15 in 15.35s. This refresh includes the newer
Infinite Lives, host God Mode, legacy save-state menu migration, NAN reticles,
initial native-menu landscape and DXR synchronization changes. Hardware
DXR-to-Vulkan sharing and portable resident-shadow checks also pass. These
results do not resolve the remaining desktop SDL DXR readback connection,
all-background visual coverage, or physical Deck/Android/headset acceptance.
No release, installation, or publication was performed during this refresh.

### Initial EX menu side-view checks

Fresh left/right native-menu Vulkan captures passed and were inspected in
tmp/vr-ex-menu-{left,right}-landscape. Both show continuous blue sky, clouds
and green ground with no moon duplication in those directions. Added a
both-eye top/bottom-row regression requiring blue sky and green ground for
the initial menu's rear/side views; the previous brown clear cannot pass.
Rebuilt tool and fresh rear assertion run passed in tmp/vr-ex-menu-rear-asserted.
Quest APK containing the prior production fix is already built, SHA256
6B6505641D6A05613AE28353FC394B2D8715DC6A14F8ADF324F31D32F09B0ED6;
this test-only follow-up does not change that APK. Installation awaits closure
of the user's running app. These are offscreen captures, not headset acceptance.

### EX native initial-menu landscape surround corrected

BG_TITLE/Mode 1 during EX pregame now selects the landscape surround with
atlas origin 248 (ground horizon 360) and the existing unique-left-half moon
policy. UI layers remain planar. Fresh native-menu front/rear Vulkan captures
passed and were inspected: tmp/vr-ex-menu-{front,rear}-landscape/live-scene-left.bmp.
Rear shows blue sky/clouds/green ground, replacing the earlier solid brown;
front retains the native menu text and one moon. EX game-input regression
passes (14.71s). These prove the initial menu atlas at the sampled state;
other menu-selectable background payloads and physical headset acceptance
remain to be checked rather than inferred from the BG_TITLE identity.

### Native EX pregame capture exposes additional background gap

New `--live-stage=TITLEMAP:menu@rear` fixture enters the actual source EX menu
through Start (51 ticks), then settles 40 ticks without pressing menu controls.
Current capture reaches reported background 525/BG_TITLE, PPU Mode 1; authored
BG2 is a landscape atlas (sky/clouds, one moon, green ground), while rear view
is solid brown. Inspected tmp/vr-ex-menu-rear-current/{authored-bg2.bmp,
live-scene-left.bmp}. Thus BG_CRED flow widening alone does NOT resolve the
user's native-menu background request. Native menu changes tile content while
the map background identity stays BG_TITLE; classification needs the actual
menu-loaded background, not blind spherical repetition of a unique planet.
This remains incomplete. The fixture permits missing OAM because EX menu
artwork is native bitmap data (5284 nonzero RAM/VRAM bytes, zero mismatch).

### EX VR NAN reticles and pregame star surround

Reticle materials now bypass NAN colour-table replacement before shape decode,
in addition to bypassing colour warp, wireframe, wobble, wave and cel pose
effects. Dedicated HUD palette and native game-plane orientation remain.
Original/EX VR game-input tests pass 2/2 (17.62s); EX checks all five NAN colour
tables for unchanged vertex position/colour/texture data. EX pregame BG_CRED
now uses the existing full star sphere, like the intro, instead of falling
through to a flat background tile panel. Other classified environment types
already select their wrap versions independent of this flow. Physical menu
background and NAN mode-1 sprite appearance still need headset acceptance.
Follow-up sprite-path regression passes against EX (13.82s): all five NAN
tables preserve XHAIR2 texels, vertex count/position/colour/texture and fixed
game-plane orientation, with wire/wobble/wave/cel flags also enabled. Latest
Quest APK built in 28s; installation is waiting because PID 10205 is still
running and the user has been asked to close it. No process was stopped.

Also fixed legacy save-state Cheats Back row migration (5 to 6 only when
the optional Infinite Lives field is absent). Regression failed before fix;
Original/EX state suites pass 3/3 afterward (33.90s).

### Broad regression refresh and Titania sample

All desktop and VR targets rebuilt. Full VR CTest passed 15/15 in 14.11s.
Desktop full CTest completed 54/54 in 300.94s. These full-suite results
precede the Infinite Lives / host God Mode override additions below.

### Infinite Lives and host God Mode priority

Added persisted Infinite Lives to desktop and VR Cheats, with localized labels,
backward-compatible state/prefs loading, and reserve-life replenishment before
death strategies. Damage/death remain enabled. VR input and both cartridge
regressions pass (3/3); desktop menu/preferences and Original native simulation
pass. Desktop targeted suite including EX native simulation passed 4/4
(136.17s); Quest APK rebuilt successfully (33s). Headset gameplay acceptance
remains separate from these automated checks.

An explicitly enabled host God Mode now survives EX native pregame choices;
the host toggle can still disable it. Native GODMODE is reasserted before
gameplay strategies. Added God Nuke player-health regression with native EX
GODMODE deliberately cleared, plus save-state coverage of the override.

Fresh Original LEVEL2_3 tick-6000 captures at 4:3/16:9/32:9 are in
tmp/issue48-current. Inspected 16:9 fills the entire image, reached gameplay
background $8d (mode 2, scroll 0/232). Exact match to issue 48's ending zone
still unproven. Background audit now retains the process handle before waiting
so PowerShell exit-code retrieval is reliable. No GitHub issue state changed.

### Original/EX surrounding screen checks and Quest update

Original Continue rear is uniform blue; Original Game Over left shows stars on
black (both inspected). Diagnostic now recognizes GAMEOVER as a host entry even
without a matching cartridge symbol. Added exact all-pixel uniform/nonblack
checks for Continue side/rear captures; fresh Original-left and EX-right pass.
Evidence tmp/vr-continue-original-{rear,left}, tmp/vr-gameover-original-left,
tmp/vr-continue-ex-right. This supplements prior EX rear views, not a complete
headset motion/fade acceptance run. Latest Quest APK (including circle/wipe
separation, timing observer fix, and surrounding screen colours) installed with
install-r Success on 2G0YC1ZF8R059K after confirming no running game process.
Saves/inputs retained; no launch, release, or visual headset claim.

### Game Over / Continue surround coverage

Continue now uses its authored BG2 corner colour for the full eye clear, as
Controls does, instead of assuming CGRAM[0]. Game Over uses the dark background
border colour; its native dust remains in the surrounding world. Forward panel,
characters and text are not replicated. EX GAMEOVER:40@from-entry@vr-world@rear
capture passed and was inspected: black space with stars, no repeated UI.
Evidence tmp/vr-gameover-rear/live-scene-left.bmp. Continue capture initially
failed the diagnostic's symbol-only validation (CONTINUE is a host scene entry);
that validation now accepts it. EX Continue rear capture now passes (uniform
blue backdrop, inspected in tmp/vr-continue-rear). Quest build passed in 25s.
No new headset install/visual acceptance; other views/cartridge still to check.

### Issue 43: stale death circle reused by respawn wipe

Fresh EX LEVEL3_2 revival captures exposed a red disk at presentation frame 600,
after stage fade/colour math had cleared. CIRCLEANIM was 357 (MSTARWIPE's running
program), but radius 171 and red colour remained from death. TRANS.ASM's WIPE_DO
returns via DOAWIPE without CIRCLECOMEND; these are window programs, not circles.
Host circle snapshot now requires DOAWIPE == 0 as well as a nonzero animation.
The native program, window geometry, death circle and bomb circle are unchanged.

Regression seeding a retained red radius then starting MSCRAMWIPE failed on both
cartridges before the fix. EX live capture after the fix retains the death circle
at frames 300/360 but removes the stale disk at 600 (same anim/radius, inactive
circle). Inspected matching before/after BMPs:
tmp/issue43-circle-trace-ex/000600.bmp and tmp/issue43-circle-fixed-ex/000600.bmp.
This establishes a concrete revival defect, not all reporter settings/platforms.

### Presentation timing observer bus-latch fix

Added full-save-state regression polling logic_tick_ready and interpolation
alpha sixteen times in Original-speed gameplay. Both Original and EX failed
before the change: pace_decision read communication/ship/boss RAM through the
emulated bus. These are now non-mutating RAM peeks, preserving values and cadence
logic without letting presentation frequency alter the bus latch. Both cartridge
game-input suites pass afterward (59.69 seconds), as do simulation unit tests.
Desktop rebuilt. This is not proof of resolution for tint/flicker reports.

### High-refresh shutter regression found and corrected

Fresh 240-Hz full SBS sequences failed: Original opening reversed at frame 102;
EX jumped at frame 170. Added test-only wipe-frame telemetry; source/interpolated
edges remained monotonic, isolating the failure after effect composition.
SDL GPU effects now reapply the horizontal clip after spatial styling/AA and
shadows, before host/menu overlays. This prevents postprocessing from bleeding
across or changing the visible shutter edge. Original and EX 480-frame reruns
pass: 117/116 sampled edge positions, fully open at frame 291/295, no reversal
and no sampled edge step above two pixels. Both eye bounds agree throughout.
Proof sequences: tmp/sbs-wipe-finalmask-original and tmp/sbs-wipe-finalmask-ex;
fail-before sequences: tmp/sbs-wipe-traced-original and tmp/sbs-wipe-traced-ex.
This verifies the desktop SDL GPU full-SBS path at 240 Hz, not every platform.

### SBS portable-shadow resident presentation

Desktop SBS portable fallback now retains two independent GPU shadow outputs
through eye effects instead of downloading/reuploading both masks. Buffers are
released before renderer/device replacement. Eye backend selection now obeys
the same hardware/software conditions as mono. Normal unsupported-hardware menu
behavior is unchanged; STARFOX_TEST_FORCE_PORTABLE_SHADOWS exercises fallback.

Rebuilt starfox_pc; Original full SBS resident/download captures are byte-identical
(SHA256 3CB4744568E09DD6FD864711885314FE7D62F21A712B1BED4DA40B8D0FD9E9D1).
EX half SBS resident/download also passes exact file-hash comparison. Evidence:
tmp/sbs-shadow-{resident,download} and corresponding -ex-half directories.
Inspected Original image: both eye views, angled ground shadow, comms and HUD.
Hardware DXR full SBS separately passes on RTX 5070 Ti Laptop GPU in
tmp/sbs-shadow-dxr-current. No portable fallback inferred from that hardware run.

One 120-frame portable comparison (60-frame warmup): resident measured stages
329/1811/71/4798 us versus download 324/4125/81/4207 us (background/world/composite/
present). This is a short local CPU-stage timing sample, not a broad FPS claim.
DXR masks still use readback in production; native D3D12/Vulkan sharing is not
yet wired through the SDL desktop presenter. Full migration remains incomplete.
No release or Quest changes in this desktop-only pass.

### EX showcase distance and intro/title music handoff

User clarified that EX's looping intro track must stop when visuals finish,
then NEWTITLE begins on the EX logo. enter_title now explicitly stops SPC/MSU
when arriving from EX intro; native BG_TITLEI continues to own NEWTITLE startup.
The intro test retains Original's natural ending check, verifies EX keeps
playing without further game ticks, then resumes simulation and requires the
stop command followed by the native NEWTITLE command. Both cartridge tests pass.

VR moves only ZACOINTRO_ISTRAT/ZACO2INTRO_ISTRAT and their running _STRAT
showcase packets 25 percent closer along viewing depth. The live capture
initially missed these because their initializer changes strategy immediately;
including the running names reaches the showcase at INTROMAP tick 337.
Backgrounds, other intro choreography and title placement are unchanged.
Both cartridge input tests pass, including EX running-strategy depth and title
isolation checks. Desktop Vulkan captures are in tmp/vr-ex-showcase-closer and
tmp/vr-ex-showcase-closer-settled (tick 400); these are not headset screenshots.
Quest ARM64 APK rebuilt successfully with all four strategies (29 seconds).
Fresh ADB inventory found Quest 3 2G0YC1ZF8R059K authorized, with no running
game process. deploy_quest.ps1 install-r reported Success; inputs/saves retained.
Not launched or headset-tested. The older friend ZIP has not been refreshed.

Full desktop CTest run ended 53/54 passing (339.10 seconds), with the obsolete
EX natural-music-ending expectation as its sole failure; corrected intro tests
now pass. VR CTest ended 14/15 passing; its injected buffer test still expected
vertex-only usage although resident ray expansion requires storage usage too.
Updated that exact usage assertion and added failure line reporting; the full
VR rerun passed 15/15 (15.89 seconds). The subsequent four-strategy change has
targeted Original/EX input passes (35.18 seconds), not another full-suite run.
Both intro-pacing tests also passed again. Full desktop suite not rerun.
All prior build/test sessions in this entry's history are terminal.

### Full regression run in progress

Rebuilt all build/current targets and the Quest ARM64 APK, now including the
latest dust/results/planet snapshot fixes. Quest build passed in 16 seconds.
Full 54-test CTest run is live in exec session 83997; re-poll that handle before
starting another suite. Original and EX level-clear tests have passed (72.91
and 84.11 seconds), and both cartridge state-data tests are now running.
The overall suite is not yet reported complete. No device install/release.

### EX dust-count getter and transition checks

Extended the full-state snapshot regression to dust_point_count. EX failed
before the native M_MORESTARS word read was replaced with a RAM peek; both
cartridge game-input suites pass afterward. Quest ARM64 built successfully
with the results/planet getter fixes before this additional dust change;
its APK does not yet include the dust fix. Both transition-parity suites also
passed, including three palette reloads on LEVEL1_1 and LEVEL3_2. No ADB device
was connected at the last check, so no installation was attempted.

### Planet presentation snapshot bus-latch fix

The active black-hole map regression seeded a distinct bus value, then read
planet/briefing snapshots. It failed on the old native CURRENTPLANET read.
planet_presentation_state now uses a RAM peek; briefing_state itself already
reads host fields only. Original and EX hitlist suites pass after the change,
including all 0-100 score glyphs, 32 tunnel phases/four widths, map cadence,
black-hole music/route state and Sector Y preservation checks. No headset
visual acceptance or broader flicker fix is inferred from this result.

### Results snapshot bus-latch fix

A new test with distinct BUNNY/FROG health bytes and a seeded bus latch failed
before the change: stage_results_state altered the complete saved emulated
state. Its teammate-health and results-exit reads now use RAM peeks, preserving
the state while providing the same display fields. This removes render-rate
dependent bus reads; it is not evidence that all timing/visual bugs are fixed.

### State-gated results proof

STARFOX_CAPTURE_RESULTS now waits for an active, visible, completed tally for
60 presentation frames, with STARFOX_TEST_FRAMES as a hard failure deadline.
The capture script uses this instead of a fixed frame that missed EX's overlay.
Fresh Original and EX captures exit 0. EX logs frame 1170, displayed percentage
15; inspected image shows completion bar and three teammate panels/meters.
Proof: tmp/results-state-gated-ex/results.bmp and
tmp/results-state-gated-original/results.bmp. These supersede the missed EX
fixed-frame capture, but do not establish Android parity or all results states.

### Current results-screen capture and GitHub refresh

Read-only GitHub refresh: #43/#44/#46/#47/#48 remain open; #49 is closed.
No new reporter comments were present. Current Windows build captures at
16:9, 2x, 60 FPS show the completion bar and lives/shield/boost/bombs in
Original. Proof: tmp/results-current-issue47-verified/results.bmp.
EX capture tmp/results-current-issue47-ex/results.bmp is outside the results
overlay at that fixed frame; it does not verify EX results. This is not
Android-device proof.
The capture script now acquires the process handle before waiting, fixing a
Windows PowerShell null ExitCode false failure. Both repeated captures exit 0.

### GPU reticle game-plane proof

Added --sprite-gameplane-gpu and --sprite-gameplane-reference to the Vulkan
scene fixture. Four size/depth cases use different nonzero head-roll angles
in both eyes. GPU shader sizing/orientation is compared with independent CPU
sizing and explicit game-plane vertices (not eye-facing billboard offsets).
All eight BMP files match byte-for-byte; case zero inspected visually.
Proof directories: tmp/vr-reticle-roll-gpu and tmp/vr-reticle-roll-reference.
Together with the real XHAIR2 packet fixture this covers the shader branch and
asset marker; physical Quest acceptance of the EX reticle remains open.

Desktop handoff inspection: current SDL_gpu.h device properties expose driver
metadata, not native VkDevice/D3D12 device or external-buffer import. The
verified separate Vulkan DXR consumer cannot simply be passed into the SDL
effects buffer binding. Production integration remains required; no private
SDL pointer casts or unverified cross-device resource substitution were added.

### DXR readback alignment check

Explicit resident-mask downloads now use one contiguous copy for aligned rows;
padded widths retain row-wise copies. Added a 256x128 aligned case between
padded/resize cases in the external-buffer hardware fixture. Fresh RTX 5070 Ti
run matches all 32768 aligned pixels across eight changing frames, alongside
the 133/257/67-width cases, alpha coverage and producer/import checks.
No controlled performance gain claimed. Desktop presentation still calls the
CPU-download path; this does not finish the resident presentation migration.

### Import permission recreation recovery

Quest import activity now saves/restores the pending Downloads permission
request across Android activity recreation. Returning from Settings can thus
resume the import instead of silently losing that action. Quest Java/ARM64
debug build passes (18 seconds). Physical permission/recreation testing remains
open; this APK has not been installed or added to the existing friend ZIP.

Expanded EX 600-frame ray-input sampling includes LEVEL1_1 (5723 accepted),
LEVEL3_3 (4617), LEVEL4_1 (5208), LEVEL4_4 (2699), and LEVEL5_1 (5672), each
with zero ray-input rejection and ordinary sprite/particle entries remaining.
These are topology/preparation checks only, not GPU pixel or full-stage proof.

### EX reticle sprite regression coverage

The input/scene test now uses the actual XHAIR2 asset in simple-scaled-sprite
mode as well as the earlier laser-mesh colour fixture. It requires nonempty
geometry and the game-plane orientation marker on every generated vertex,
covering the branch that must prevent eye-facing rotation under head roll.
Fresh EX game-input suite passes. This is packet-level regression evidence,
not a rolled-headset pixel comparison or acceptance of the reported fix.

### EX controls ship colour report remains open

Fresh EX CONTMAP tick-40 capture: all sixteen model working-palette words equal
CGRAM entries 112..127. No palette substitution was applied: that proposed
cause is contradicted in this direct-entry fixture. Capture at
tmp/vr-ex-controls-palette-audit/live-scene-left.bmp. Material/source lighting,
sRGB presentation and the reported headset appearance still need comparison
before calling the user's colour report fixed. The new
--live-controls-from-title fixture reaches controls through title/EX menu after
66 ticks, then captures after 40 controls ticks. All sixteen working/CGRAM
palette words also match on this route. Native Vulkan checks pass; capture:
tmp/vr-ex-controls-title-route/live-scene-left.bmp. This does not establish
correct headset colour presentation or justify a palette override.

### Compute fallback audit and empty-controller preparation

SourceModelPackets now retains compute producer rejection reasons separately
from fatal missing-render entries. Ray audit reports are keyed by strategy,
shape, reason and render flags as well as object slot; reused pool slots no
longer hide later failures. EX 5-1 exposed repeated empty NULLSHAPE controller
preparation, reported misleadingly as buffer bounds failures. Empty-face models
now skip compute packing while keeping the existing empty geometry path.

Fresh EX LEVEL5_1 120-frame audit: 1254 compute model/frame entries accepted,
zero ray-input rejection, 144 ordinary legacy particle/whole-sprite entries.
EX LEVEL4_4 240-frame audit: 824 accepted, zero rejection/legacy. These are
input/topology coverage checks, not GPU rendering or full-stage completion.
Original LEVEL1_7 preflight failed route initialization; it is not verified.

### EX intro comms shared compositor capture

Extracted the app's unchanged portrait/text/meter assembly into
source_dialogue_packets and reused it in the Vulkan capture tool. The tool
previously omitted the app's replacement comms overlay, so its output was not
evidence about live comms. It now also suppresses the replaced EX bitmap.

EX INTROMAP @first-dialogue reaches the alternate communication channel at
tick 9 with three visible layers (portrait and two text passes), meter off.
Both-eye Vulkan suite and EX input suite pass. Inspected capture:
tmp/vr-ex-intro-comms-shared/live-scene-left.bmp. This verifies shared GPU
composition offscreen, not headset acceptance or every later message.

### Native grid stage correction and comms review

Correction to the earlier commentary: LEVEL3_5 is Macbeth, not Titania.
The actual Titania LEVEL2_3 tick-400 entry now also passes the both-eye
terminal-floor comparison. Its native DOTSFLAG is -1 (airborne particles),
as is Fortuna LEVEL3_3; Macbeth LEVEL3_5 uses +1 ground dots. Capture fixtures
now assert these source modes so shadows cannot silently force a grid.

Reviewed FRIENDS_MESSAGES_L meter handling against the shared dialogue snapshot:
ordinary messages clear FRIENDS_METER; requested meters use its high-bit-tagged
health and only show after the portrait opens. Existing source-state tests cover
ordinary/requested/opening/closing/Fox cases. Fresh VR packet tests pass the
portrait aspect and meter geometry checks; live EX intro comms acceptance is
still not established by these tests.

### User background-scale acceptance

The user reports background scale looks good in their friend's playthrough.
Keep the scale unchanged. This is acceptance for the scenes they observed,
not proof of every stage or resolution. Original LEVEL3_3 (Fortuna) and
LEVEL3_5 terminal-ground checks also passed in both eyes. Source BG_3_3A uses
pollen (DOTSFLAG -1) despite shadows; do not force ground dots into that scene.

### Ground transition-row extrusion corrected

The EX 5-1 lower-floor stripes came from clamping logical Y at 223: with
atlas origin 232, source row 455 (two alternating colors) was stretched across
the lower hemisphere. Ground projection now continues through the remaining
atlas rows and clamps at the terminal floor row rather than the window edge.

Fresh @terminal-ground-check fixtures pass for EX LEVEL5_1 and Original
LEVEL1_1 at entry tick 400 in both GPU eyes, comparing the bottom-center area
against the decoded solid terminal atlas row. EX visual capture:
tmp/vr-ex-ground-terminal-verified/live-scene-left.bmp. Full landscape coverage
and headset-scale acceptance remain open; these two fixtures are not all-stage
proof. Quest APK rebuilt successfully. Updated friend archive:
tmp/Starfox-Quest-Preview-2026-09-12-import-fix.zip (no ROM/BIN included).

### Missing Quest picker confirmed / EX atlas decoder checked

ADB package resolution on Quest 3 returned no handler for OPEN_DOCUMENT or
GET_CONTENT. Added an explicit Downloads import fallback. It requests optional
all-files access through Android Settings (a handler was confirmed), then reads
only Download/Starfox-Assets.BIN through the existing bounded validator and
atomic installation. Permission denial leaves retry/manual-copy instructions;
no permission was granted by tools. Fresh-install visual acceptance remains open.

EX LEVEL5_1 tick 400 @atlas-check now samples all 256 lower atlas rows at varied
columns. Both GPU eyes exactly matched the CPU decoder in all 256 cases.
The distorted-looking lower ground persists in the live projection capture;
tile decoding is not supported as its cause by this test. Projection/aliasing
and actual headset scale still need investigation.

### Friend startup, menu punctuation, and native grid follow-up

Missing-BIN startup now redirects from the immersive VR activity to a separate
non-VR AssetImportActivity and offers ACTION_OPEN_DOCUMENT on first resume.
Cancel leaves an import button, and Start VR returns to QuestActivity after
validation. Java/manifest compilation passed; a fresh-install headset picker
test is still required (the connected headset's existing assets were preserved).

Host ASCII packets now define colon, slash, and selection-chevron ink rather
than using cartridge aliases. Original and EX input suites passed exact packed
row assertions. Unsupported ray tracing is omitted from the 3D page, with its
row count and Back behavior covered by the passing VR input tests.

Per the latest native-grid request, the VR world assembly no longer replaces
connected grid lines with dots or substitutes landscape tilt for the native
camera matrix. Native DOTSFLAG/GRIDLINE state continues to gate visibility.
Full stage-by-stage visual grid parity remains unproven.

### Quest EX candidate installed (2026-09-12)

Rebuilt and installed the pending EX native menu/intro bitmap, intro dialogue,
fixed-plane reticle, and GPU ground projection candidates on Quest 3
2G0YC1ZF8R059K using install-r; assets and saves were preserved. This is not
headset visual acceptance. The latest headset screencap was unreadable, so it
does not prove any rendering fix. The friend ZIP remains older.

Fresh packet and Vulkan scene checks passed. EX LEVEL5_1 at entry tick 60
renders the orbital surface below the horizon in
tmp/vr-ex-orbital-followup/live-scene-left.bmp. The reported live right-side
surface remains unverified. EX ground stripes, background scale, text, and
head-roll reticle behavior still require visual acceptance.

The live-stage capture tool now defaults to GPU ground deformation like the
headset. Use @cpu-ground explicitly for comparisons; previous CPU-default
captures were insufficient evidence for the headset path.

### Fade cache and comms snapshot follow-up

The new VR comms capture exposed a pre-existing side effect in dialogue_state:
read_native_byte/word updates the emulated bus latch even through const access.
Changed this presentation getter to RAM peeks. The previously failing complete
save-state equality check now passes in Original and EX.

SourceModels geometry cache now includes display brightness; raw palette words
alone cannot distinguish a display fade. Fresh vr_game_input checks pass both
cartridges with cached/uncached geometry comparisons at 15/7/0/15 brightness,
forced-black grid assertions, complete-state parity, menu experience lock and
90 Hz frame/audio pacing. Full title-to-controls visual acceptance is still open.

### Latest EX intro / VR transition / training candidate

EX intro now selects DO_BGM_FITUNL, matching the upstream ENDSEQ.ASM
INTRO_L bgm2 fitunl instruction; Original keeps DO_BGM_INTRO. Shared-core
transition checks and EX bundle INTROMAP preflight pass; listening proof pending.
VR model/grid/dust palettes now apply source display brightness, matching the
2D layers during fades and forced black. This addresses a concrete early-model
visibility cause, not yet complete title-to-controls headset acceptance.

EX BG_TRAINING uses stars/stars and nodots in upstream BGS.ASM; it must not
inherit Original training's ground projection. It now selects the star sphere
and explicitly excludes landscape deformation. TRAININGMAP:400 with held Down
passes both cartridges, including explicit environment assertions. Inspected
captures: tmp/vr-ex-training-stars-fixed/live-scene-left.bmp (EX height -615),
tmp/vr-original-training-regression/live-scene-left.bmp (Original landscape).
These are offscreen Vulkan captures, not headset photographs. Fresh Quest ARM64
APK build passed with all three changes; the earlier friend ZIP predates them.

### Bundle-backed Quest deployment (September 12, latest)

Supersedes the packaging status below: Quest now imports a single
Starfox-Assets.BIN, validates it before atomically replacing the installed
bundle, and loads Original/EX directly in memory. JNI validator includes and
import library-error recovery are fixed. Fresh ARM64 APK build passed.
Desktop bundle-backed preflights passed LEVEL1_1 for Original (225 packets)
and EX (207 packets), two logic ticks after checkpoint warmup, with three
interpolation samples per frame. These are not headset visual proof.

Installed the APK and existing validated 4,912,472-byte companion on Quest 3
2G0YC1ZF8R059K. No saves or loose source files were deleted. Launch was
intercepted by LaunchCheckControllerRequiredDialogActivity, not the game;
headset runtime acceptance and document-picker lifecycle testing remain open.
Native diagnostics now expose --bundle-preflight and --bundle-ex-preflight.

### VR asset companion packaging remains incomplete

Native VR --bundle PATH now reads a bounded 64 MiB companion, validates
against embedded runtime_companion_manifest, constructs Original in memory,
and sources EX/level choices from the decoded payload. It rejects mixed loose
inputs and bundle mode. Windows and Quest ARM64 builds pass; missing-file
failure verified. Quest Java picker/JNI still needs connecting, and valid/stale
bundle lifecycle/runtime acceptance remains pending. No loose files removed.

Added cmake/VRAssets.cmake to generate VR validation resources independently
of STARFOX_BUILD_RUNTIME; Quest now enables STARFOX_EMBED_RUNTIME_ASSETS.
Inputs are BPS patches and symbol tables only, with explicit missing-resource
failure. ARM64 build is being verified before treating this wiring as complete.

Shared runtime_companion_manifest now owns the exact resource order and text
normalization policy. Desktop and asset builder call it; missing resources
reject explicitly. Fresh BPS/manifest tests pass and asset builder compiles.
VR can reuse this function once its embedded resource target is linked; the
bundle picker/decoder integration is not yet finished.

LiveGame now owns in-memory RomImage/SymbolMap inputs through a common
constructor; the development file-path constructor delegates to it. Runtime
build and two-tick Original/EX preflights exercise this ownership path without
writing extracted files. Bundle decoding, shared manifest resources and the
Quest importer UI are still not connected; this is preparatory work only.

QuestActivity currently imports SF.SFC/SYMBOLS.TXT and SFES.SFC/SFES-SYMBOLS.TXT.
Quest Gradle explicitly disables STARFOX_EMBED_RUNTIME_ASSETS and the runtime
target, so the desktop's embedded manifest resources are not currently linked.
The existing RuntimeBundlePayload already contains both prepared images and
symbol tables. Production VR should load Starfox-Assets.BIN through
decode_runtime_bundle with the same expected manifest as desktop, then build
both experiences in memory. Do not extract loose images as a new requirement,
accept an arbitrary bundle manifest, or remove existing user data during the
transition. Desktop manifest uses resources 101,102,108,109,120 through 126
with text newline normalization for 102/109. Implementation and lifecycle
tests for missing/stale/corrupt/valid bundles remain required.

### Revised tunnel scope (supersedes earlier center-window containment)

Fresh Quest ARM64 build succeeds after draw-order change. EX5-1 source
captures at 60 and 120 ticks from entry both show the planet below a horizontal
horizon, not on the right (tmp/vr-ex51-scramble-right and
tmp/vr-ex51-scramble-120). Both inspected. This does not reproduce the live
headset report; investigate the live view rather than rotating the atlas on
the strength of a non-reproducing offscreen fixture.

User clarified that gameplay must remain full 3D, including final-boss areas.
Only duplicate tunnel background artwork should be suppressed at sides/rear.
Live VR now draws the solid surround after backgrounds but BEFORE models,
shadows and explosions, instead of masking them afterward. Diagnostic order
matches; side/rear black-only assertions now apply only to model-free captures.
Windows runtime build passes. Full rendered coverage and final-boss audit,
Quest rebuild/install and EX5-1 scramble planet placement remain pending.

### Latest headset reports: dialogue, background scale, grid

VR replacement communications now resolve Original/EX translations through
the retained ScaledTextRenderer layout, using desktop 92-pixel wrapping and
10-pixel translated line spacing. English/Europe keep native text fallback.
Windows runtime build and Quest ARM64 build pass; existing simulation test
executable passed with both cartridges, including translation line-width
checks (not a rendered VR proof). Quest install-r succeeded with saves intact;
this latest install includes EX replacement and localization, not yet visually
accepted. Deep background stretching and camera reports remain unresolved.

EX active gameplay/training communications now use the shared GPU portrait,
text and conditional meter path, matching desktop replacement policy. The
native BG1 is suppressed only while replacement is active, not on pause or
stage results. Snapshot captures paused state; policy tests cover hidden,
active, paused, results and training. Fresh runtime and packet builds pass.
This change postdates the last Quest install; actual EX dialogue rendering
and localized text still require verification/work.

Mothership-exit investigation: fresh Original LEVEL1_1:60@from-entry@front
capture reports source camera (-536,-938,3921), rotation (54176,15616,0).
The source is not frozen at zero rotation in this fixture. Added full camera
rotation logging to the scene diagnostic. Capture:
tmp/vr-exit-camera-60-current. This does not clear the user's live VR report;
live presentation/cinematic motion still needs comparison across frames.

Latest deployment: Quest ARM64 debug build succeeded in 22 seconds; install-r
on authorized Quest 3 2G0YC1ZF8R059K succeeded, preserving saves/inputs.
QuestActivity cold launch succeeded (609 ms). This installation includes
Original dialogue text/portrait/conditional meter and the orbital horizon
scale adjustment below. Launch is not visual acceptance or gameplay FPS proof.

Original VR teammate meter is now composed only for active dialogue with
meter_visible. It uses the desktop frame/fill coordinates and palette indices,
leaving the interior transparent; zero health keeps only the frame. Fresh
packet tests cover inactive/hidden/empty/partial/full/out-of-range health and
the runtime builds. The simulation already gates meter_visible by source
FRIENDS_METER, teammate ID and message animation phase and suppresses dialogue
during player death. EX continues using its native bitmap path. Rendered
comms sequence and headset acceptance are still required.

Original VR dialogue now also submits the source portrait while dialogue is
active. FACEDATA tiles remain 4bpp packed for the GPU background decoder;
palette 112 backing preserves opaque zero pixels. Width is corrected by 7:6
with the right edge fixed at x=80 beside the text. Fresh runtime build and
packet tests pass (word-addressed tilemap, tile ordering, backing index,
geometry bounds). Actual rendered portrait comparison/headset proof and
conditional teammate meter remain pending; EX still uses its native layer.

Orbital surface horizon mapping no longer linearly stretches 15/40/80 atlas
rows across pi/2 radians. It now starts at the native 256 texels/radian and
smoothly approaches the strip's lower bound. Fresh packet tests verify first
ring density for normal/thin variants and existing surface bounds; full scene
diagnostic passes. Inspected Original LEVEL2_2:400@from-entry@front capture in
tmp/vr-orbit-scale-current: surface and horizon present, but deeper surface
continuation still visibly stretches. Not a complete background-scale fix,
not Quest-deployed, and does not address landscape mountains yet.

Cartridge captures at LEVEL2_3:900@from-entry@front select dots_mode=-1
in both Original (background 141) and EX (background 201), with 120 source
star points and 240 submitted quads. The grid renderer intentionally rejects
negative dust mode. Evidence: tmp/vr-23-grid-current and
tmp/vr-ex23-grid-current, produced by starfox_vr_scene_check. This explains
the sampled absence without proving all stage transitions; do not force a
ground grid into this star-mode scene based on the report alone.

Original VR now snapshots DialogueState and composes its visible message text
with the source message ink and shadow ink, using the desktop coordinates.
EX retains its existing BG1 path to avoid double text. This is only the missing
Original text path: portrait/meter coverage, translations, EX missing-comms
reproduction and headset captures remain open. Oversized/squashed backgrounds,
2-3 grid absence and scripted/vertical camera reports remain unresolved.

### Remaining 360-degree backgrounds / tunnel containment

Fresh `--tunnel-surround` stereo test passes in forward, left, right and rear
directions (eight captures in tmp/vr-tunnel-surround-proof). A green clear plane
is visible only through the forward opening; every other pixel must match the
requested solid surround RGB within one quantization step. Forward/rear left
eye captures visually inspected. This proves enclosure coverage/color in a
synthetic scene, not actual tunnel gameplay alignment, transitions or head
translation beyond the enclosure. Cartridge/headset captures remain required.

Added a depth-disabled opaque tunnel enclosure after world geometry and before
HUD: four front strips leave a 256x224 source opening, and five surrounding
faces cover side/rear geometry. Placement follows the source-layer origin;
color uses the same border palette/fade. Leaving a tunnel uploads an empty
surround. Fresh application and packet tests pass (geometry, placement, color,
invalid input), but stereo forward/side/rear captures and headset verification
remain required before this can be claimed visually complete.

User requires every remaining background completed before handoff, with tunnel
gameplay confined to the central view and a solid border-matching surround.
Application now gives tunnel classification precedence over intro/star/landscape
spheres and uses the same darkest-palette border selection as wide tunnel tile
margins for the VR clear color (not potentially colored CGRAM zero). Fresh
application build/tests pass. This is not the complete tunnel requirement:
geometry outside the central source view still needs containment, and all-level
360-degree captures plus transition checks remain. Not yet headset-installed.

### Whole-object sprite sizing (September 12)

Dedicated `--sprite-gpu` / `--sprite-reference` rendered fixtures now compare
GPU sizing with the pre-migration double-precision CPU formula. All eight
stereo BMPs match byte-for-byte: ordinary size (64/512), capped size at the
minimum accepted depth (1000/128), subpixel disappearance (1/32768), and just
past a truncation boundary (64/512.001). Visible cases require nonzero green
coverage and the subpixel case requires no lit pixels. Evidence:
tmp/vr-sprite-sizing-gpu and tmp/vr-sprite-sizing-reference. This does not prove
every possible double-to-float boundary or physical headset behavior.

Whole-object sprite packets now carry corner signs, source size and source
depth; the vertex shader applies native projected-size truncation and the
240-pixel cap instead of receiving CPU-sized quads. Texture decoding/palette
packing and near-depth submission culling remain CPU work. Regenerated SPIR-V,
fresh packet tests and the existing Vulkan scene suite pass
(`tmp/vr-sprite-size-shader-check`). Dedicated rendered boundary comparisons
for this new flag are still needed; the existing suite is not that proof.
This change postdates the latest headset APK installation.

### Quest deployment (September 12, latest)

Fresh ARM64 APK build passed and adb install -r succeeded on Quest 3
2G0YC1ZF8R059K, preserving application data. Launch was intercepted by Meta's
LaunchCheckControllerRequiredDialogActivity, not accepted as a successful game
startup. Wake/connect controllers and dismiss the headset prompt before visual
or performance acceptance. This installation includes the new axis/warp path;
it does not establish headset rendering parity or close the overall goal.

### Axis-collapse GPU migration (September 12)

Combined axis/warp is now connected through SourceModels and VulkanSourceScene.
The seeded `--axis-gpu-warp` fixture produces the same first-face line material
as the independent reference: all four visible/behind stereo BMPs match exactly
(`tmp/vr-axis-warp-render` versus `tmp/vr-axis-render-reference`). The scene owns
both producers and retains resources on repeat. Axis input validation accepts
the live caller's matching original vertex count, while retaining mismatched
count rejection. Actual cartridge intro combinations and Quest-device output
still need coverage; this fixture is not an exhaustive random-material sweep.

Combined resident axis/warp now initializes and dispatches in VulkanSourceModel.
The fresh native scene diagnostic (`tmp/vr-axis-warp-resident-fixed`) verifies
exact transformed endpoints, the first generated random line material, no
texture sampling and zero unchanged-input uploads with both stages sharing one
arena. Initial failure was the diagnostic retaining an unclipped layout while
requesting expanded warp; it now supplies the required occurrence layout and
all producer stages. Live SourceModels/SourceScene still exclude the combined
mode pending rendered-reference coverage; this is not a full migration claim.

Near-plane crossing and wholly-behind fixtures also match byte-for-byte in both
eyes (`tmp/vr-axis-near-gpu` / `tmp/vr-axis-near-reference`). Axis/warp input
preparation now preserves the all-vertex PRNG seed, emits only the authored
first-face material, disables texture sampling and bypasses destruction.
Fresh packet tests pass, including wrapped seed arithmetic. Combined resident
axis/warp bindings and live dispatch are still not enabled.

Added `--axis-gpu-matrix` / `--axis-reference-matrix` for a nonidentity Q15
rotation matrix with continuous geometry. All four front/behind stereo BMPs
match byte-for-byte in tmp/vr-axis-matrix-gpu and tmp/vr-axis-matrix-reference.
This closes the single-matrix fixture gap, not an exhaustive transform sweep.

Added fractional Euler rotation render fixtures (`--axis-gpu-rotated` and
`--axis-reference-rotated`): pitch 17.25, yaw 34.5, roll 26.75. All four
front/behind-camera stereo BMPs are byte-identical in tmp/vr-axis-rotated-gpu
and tmp/vr-axis-rotated-reference. Quest APK rebuilt successfully with the live
axis integration; not installed or headset-verified. Near-plane, matrix mode,
and combined colour-warp acceptance remain open.

Live SourceModels now emits axis compute requests (except combined colour warp),
and VulkanSourceScene owns a lazy shared axis pipeline, validates mode/layout
changes and retains compatible buffers. Scene-owned rendered axis output is
byte-identical to the four reference BMPs in `tmp/vr-axis-scene-proof`; resource
reuse and freshly rebuilt Original/EX input tests pass. Actual intro capture,
rotation/near-plane coverage and Quest acceptance are still required. Combined
axis/warp remains excluded; no full-migration completion is claimed.

prepare_source_axis_model now retains all projection points but emits one line
material from the first authored face, bypassing BSP/visibility and fragment
expansion. Tests deliberately supply an invalid unused BSP root and a second
red material: first-face green wins. All four rendered axis BMPs exactly match
the reference in `tmp/vr-axis-first-face-proof`. Live scene hookup remains.

Axis endpoint graphics now resolve packed even/odd material colours through
the resident source palette (including optional sRGB decode and dither). The
render fixture supplies a deliberately red fallback while its cartridge material
selects green; all four axis BMPs still exactly match the CPU reference in
`tmp/vr-axis-palette-proof`. Multi-face authored-first-face preparation and live
scene integration remain pending; this fixture has one face.

Dedicated `--axis-gpu` rendered fixture now exercises projection, reduction and
resident endpoint drawing, including front-facing and behind-camera cases.
All four stereo BMPs are byte-identical to the existing CPU-axis reference:
`tmp/vr-axis-render-gpu/axis-*.bmp` versus `tmp/vr-axis-render-reference/axis-*.bmp`.
The visible left-eye capture was inspected. This proves the simple unrotated
fixture, not arbitrary cartridge axes, authored material selection or live hookup.

Axis models now build two immutable line templates addressed to resident
endpoints, and have a dedicated vertex-shader fetch/coordinate conversion and
line-only graphics branch. Shader generation/build and template address tests
pass; existing stereo regressions pass in `tmp/vr-axis-graphics-template-proof`.
This is not yet a rendered axis proof: a dedicated capture, authored material
selection, and live scene selection remain before enabling cartridge axes.

Resident VulkanSourceModel now optionally owns appended axis storage/bindings,
dispatches reduction immediately after projection, updates validated inputs in
place and exposes bounded diagnostic endpoint readback. Real projection-to-axis
dispatch produces the expected two transformed group centres; repeated identical
updates upload zero bytes. Full diagnostic passes in `tmp/vr-axis-resident-proof`.
This producer is opt-in only: endpoint graphics consumption and live scene
selection are not enabled yet; combined axis/warp is still rejected explicitly.

Validated axis input-only writes now upload indices/uniforms without touching
GPU endpoints/residuals. Packet tests cover exact payloads and transactional
rejection of overlapping/truncated layouts. The owned-binding Vulkan fixture
uses these writes and still matches the direct-binding endpoint reference;
full diagnostic passes in `tmp/vr-axis-upload-proof`. Live integration pending.

VulkanAxisBindings now owns descriptor sets for a borrowed combined arena and
axis pipeline, checks producer/output bounds before allocation, and records
endpoint dispatch with a consumer barrier. Executed comparison against direct
bindings is endpoint-byte-exact for valid, invalid-index and empty-group cases;
truncated producer rejection and reinitialization pass. Full stereo diagnostics
pass in `tmp/vr-axis-bindings-proof`. Live model ownership and drawing remain.

Axis arena allocation now appends indices, two endpoints, two residual records
and uniforms after the projection prefix, respecting physical-device alignment
and descriptor limits. Packet tests verify exact offsets, unchanged output on
failure, invalid indices/groups, bad alignment, prefix overflow and undersized
descriptor limits. Tests pass. This layout is not yet bound in live scenes.

SourceAxisInputs now prepares authored extrema indices and the shader's 48-byte
uniform transactionally, with projected-count agreement and per-group limits.
Packet tests cover unequal extrema membership, coplanar groups, invalid mode
and unchanged output on rejection. The freshly rebuilt Vulkan dispatch now
consumes this preparation rather than handwritten uniform/index records;
`tmp/vr-axis-authored-proof` passes. Live arena/consumer integration remains.

Added the shared desktop axis shader to native Vulkan's source compute stages:
three input buffers, two endpoint outputs, a 48-byte uniform and a single
two-lane dispatch. Rejects incorrect endpoint dispatch counts. Intel Vulkan
dispatch tests verify transformed-group averages and projected endpoints with
unequal depths (not the average of projected positions), invalid indices and
empty-group rejection. Full stereo diagnostic also passes in
`tmp/vr-axis-dispatch-proof`. This is an executed compute producer, not live
intro completion: source arena/binding integration and endpoint graphics
consumption still need connecting. Existing live axis exclusion remains.

### VR preferences (September 12)

The shared VR startup/runtime menu now saves changed language, audio, pacing,
crosshair, button-swap and cheat preferences on Start/Resume, next to the host's
cartridge save as `vr-preferences.bin`. Uses atomic replacement; invalid length,
version, enum and boolean values reject the entire record without partial edits.
Read/write failures are nonfatal. Level jumps, experience selection, navigation
and device availability are deliberately session-only. Hosts without a save path
remain nonpersistent (including diagnostic runs). Round-trip, corrupt-field,
truncation and transient-state exclusion tests pass; shared application builds
and Quest APK build pass. Headset restart acceptance remains pending. This does
not complete the missing VR 2D/3D effects menu pages or full migration.

This checklist tracks the user's expanded objective, not a release-completion claim.
Nothing is published until the requested work and verification are finished.

## VR ground/grid update (2026-09-12)

- Warp graphics metadata no longer constructs full source/warp upload lists.
  It validates layouts and writes only the required 80 bytes. Host 256-update
  microbenchmark: 7.88 to 0.40 us/update (~20x for this step, not game FPS).
  Packet/Vulkan tests pass; 106 before/after BMPs are identical
  (`tmp/vr-warp-metadata-{before,after}`). Quest rebuild/measurement pending.

- Rebuilt every registered VR test executable. Fresh Original/EX input tests
  exposed stale removed-grid expectations; updated them to verify restored
  grid/star prefixes, fixed-height grid geometry, painter order and compute
  indices. All 14 rebuilt VR tests now pass. Quest APK builds with current
  warp/destruction work; headset installation/acceptance still pending.

- Resolved the destruction capture mismatch as a reference-mode mismatch:
  resident VR uses continuous vertices with Q15 fragment directions when
  subpixel projection is off; the old reference quantized both. Added an
  explicit matching-mode reference. Both eye BMPs match byte-for-byte between
  `tmp/vr-fragment-native-gpu` and `tmp/vr-fragment-gpu-mode-reference`.
  No production rendering change; fully word-quantized legacy output remains
  distinct by design. Broader cartridge/combined-warp visual parity pending.

- Added GPU/reference destruction capture modes. Rotated single-face fragment
  fixture matches both eye BMPs byte-for-byte in continuous projection
  (`tmp/vr-fragment-render-{gpu,reference}`). Source-quantized captures differ
  in both eyes (`tmp/vr-fragment-native-{gpu,reference}`); investigate before
  claiming native parity. Continuous GPU left capture inspected.
  This is synthetic fixture parity, not all cartridge destruction acceptance.

- Combined warp/destruction now enters GPU assembly with fragment-remapped
  corners and sprite-centre visibility, preserving original vertex-count seed
  and destruction sequence mode. Packet seed/topology tests and all three
  cartridge combined initialization/dispatch checks pass
  (`tmp/vr-warp-fragment-proof`). Dedicated rendered parity and headset testing
  remain pending; these checks do not prove every destruction visual correct.

- Fragment displacement regression now includes a quarter-turn Q15 matrix and
  passes signed component-wise source expansion checks. Enabled ordinary
  destruction in cartridge GPU assembly; all three cartridge fixtures retain
  fragmented inputs without CPU geometry duplicates. Host suite passes in
  `tmp/vr-live-fragment-assembly-proof`. Combined warp/destruction, rendered
  destruction parity, and Quest rebuild/acceptance remain pending.

- Added direct GPU destruction-position regression: two opposing normals,
  identity Euler/matrix modes, progress 1/4/31, and all XYZ offsets compared
  against source signed expansion arithmetic relative to non-destroyed GPU
  positions. Tolerance 0.001 source units; passes with full host diagnostic
  (`tmp/vr-fragment-position-proof`). Rotated/visual/warp-combined cases and
  live assembly enablement still pending.

- VR span preparation now packages flattened destruction topology and shared
  per-face transform inputs, with explicit fragmented pose sizing. Packet
  allocation tests pass; three cartridge models dispatch Euler and matrix
  destruction with nonempty spans (`tmp/vr-fragment-input-proof`). Live assembly
  still excludes destruction pending positional/visual parity and combined
  warp-destruction handling. This is not proof of fragment motion correctness.

- Extracted desktop continuous destruction input expansion into shared
  `pack_continuous_fragments`, retaining GPU-side displacement/rotation and
  sprite-centre visibility inputs. Desktop GPU model now uses that helper.
  Pose/corner/visibility regression passes; SDL-enabled desktop core builds.
  VR destruction allocation, shader settings and dispatch remain unfinished.

- Fixed scene reuse when warp coordinate storage changes size: compatibility
  checks now trigger reconstruction rather than failing the retained update.
  Grow/restore/unchanged regression passes through GPU dispatch and stereo
  draw (`tmp/vr-warp-resize-proof`). Compatibility validation does not copy
  texture payloads; packet and Vulkan tests pass. Not yet rebuilt for Quest.

- Warp line graphics now suppress texture sampling, matching native two-point
  face material behavior. Packet/cartridge/Vulkan suite passes at
  `tmp/vr-warp-line-material-proof`; dedicated randomized-line visual fixture
  remains needed. Quest debug APK builds successfully with current migration
  and restored grid/ground work. ADB lists no device: not installed or tested
  on the headset; no performance/visual acceptance claim.

- Enabled mixed polygon/line/sprite warp requests using retained primitive
  flags and the renderer's mixed draw path. ROBOT_0 no longer reports pending
  warp assembly. All three cartridge fixtures initialize and dispatch the
  resident chain with successful nonempty traversal (`tmp/vr-mixed-warp-proof`).
  This is not visual parity proof for warp lines/sprites; dedicated comparisons
  and Quest deployment remain pending, as does destruction migration.

- Cartridge assembly now emits resident warp requests for polygon-only warp
  models. Host diagnostic routes BOSS_H_2 and MY_DEMO successfully, retaining
  empty CPU placeholders and validated expanded allocation. ROBOT_0's mixed
  primitives still report the existing native warp pending error; this remains
  incomplete. Proof: `tmp/vr-cartridge-warp-proof`. No Quest deployment or
  all-stage warp parity claim; mixed primitive/destruction paths remain.

- Ordered scene renderer now accepts warp requests, lazily owns/shared-caches
  all three warp pipelines, updates resident models, and distinguishes warp
  mode changes during reuse. Scene-owned stereo case 19 and retained-resource
  initialization pass (`tmp/vr-scene-warp-proof`). Cartridge model assembly
  does not yet emit these warp requests; gameplay enablement remains pending.

- Owned warp models now support same-layout per-frame delta updates. Host
  diagnostic proves zero-byte unchanged updates, a 16-byte seed-only update
  with allocation reuse, incompatible-capacity rejection, and restored output
  equality followed by stereo pixel checks (`tmp/vr-warp-update-proof`). Live
  scene selection and shared warp pipeline lifetime are still unconnected.

- `VulkanSourceModel` now optionally owns the combined warp arena/descriptors,
  schedules warp after BSP and before clipping, and exposes its expanded
  geometry through normal graphics preparation/drawing. Owned output commands
  match the explicit chain; stereo case 19 passes exact pixels in
  `tmp/vr-owned-warp-proof`. Per-frame warp updates are explicitly rejected
  until implemented; live scene selection/shared warp pipeline lifetime pending.

- Resident warp graphics proof now retains the combined producer/warp arena
  through both eye draws, using the shared graphics lookup. Case 19 verifies
  exact two-face coverage/colors without geometry readback/re-upload; host
  Vulkan diagnostic passes (`tmp/vr-warp-draw-proof`, 106 BMP captures).
  First-run pipeline compilation stalls were logged; no FPS claim. Live scene
  ownership/update integration and repeated-face projection coverage remain.

- Added shared transactional warp graphics metadata writer, routing results,
  polygons, corners and materials to expanded buffers while preserving authored
  BSP order/projection inputs. Diagnostic validates each routing offset and
  rejection retention; packet/Vulkan tests pass (`tmp/vr-warp-lookup-proof`).
  This diagnostic currently dispatches the compute chain, not a draw of that
  combined warp arena; live draw ownership/integration remains pending.

- Corrected occurrence graphics projection lookup to map through authored BSP
  face order; projection inputs are not expanded alongside warp geometry.
  Regenerated VR shader binaries and passed host Vulkan diagnostic at
  `tmp/vr-warp-projection-proof`. A repeated-face graphics fixture with distinct
  projection settings is still needed for direct coverage of this mapping.

- Added explicit expanded-warp allocation mode: authored topology/material
  inputs retain source-face sizing while clipping/span outputs use traversal
  capacity. Regression covers output count greater than authored faces and
  validated upload generation. GPU warp diagnostic uses this mode without a
  manual clip-uniform patch. Ordinary model submission rejects this mode until
  live warp bindings/graphics routing are integrated.

- Warp bindings now validate six producer descriptor sizes against declared
  input counts before allocation/dispatch. Truncation/recovery regressions and
  the full host Vulkan diagnostic pass (`tmp/vr-warp-prefix-proof`). Expanded
  live output arena sizing and gameplay integration are still required.

- Removed unnecessary geometry recomputation from direct palette-only GPU
  updates. Regression verifies completed output remains reusable and palette
  bytes change without altering span buffers. Host Vulkan suite passed in
  `tmp/vr-palette-reuse-proof`; no headset FPS improvement is claimed.

- Warp consumer diagnostic now binds expanded occurrence corners/polygons,
  traversal results and materials directly into clipping/span generation.
  Both occurrences emit exactly eight rows with their distinct generated
  colors. Host Vulkan diagnostic passed in `tmp/vr-warp-consumer-proof`.
  This is a resident compute-chain proof, not live gameplay enablement or
  a headset rendering/performance claim; live arena sizing remains pending.

- Restored the landscape grid with its original lateral/distant extents and
  shared landscape transform; vertical steering does not introduce view pitch.
- Lower landscape background vertices now form a flat ground plane at the
  grid reference height instead of a lower sky hemisphere (negative landscape
  camera-height cases). Sky artwork remains on the surround geometry.
- Native packet regressions pass; Corneria local stereo capture inspected.
  Quest debug APK builds successfully. Headset is disconnected, so this update
  is not installed and physical ground alignment remains unverified.

## Current completion blockers (source audit, 2026-09-11)

These take precedence over historical progress entries below; the goal remains
incomplete. A passing regression suite does not remove these source-level gaps.

- Ordinary mono GPU gameplay now records native model geometry by default.
  `STARFOX_DISABLE_GPU_GEOMETRY` retains a diagnostic hybrid comparison path;
  software rendering remains available. Original/EX Corneria final captures
  match the CPU reference at 1x/2x/4x without a geometry opt-in flag. Broader
  stage/performance and non-Windows runtime acceptance remain required.
- DXR resident output is a native D3D12 buffer, while SDL presentation has no
  public native-buffer import hook in the bundled header. Runtime shadows still
  download masks; cross-device GPU presentation has not been implemented.
- Live VR now routes eligible ordinary polygon, line and sprite models through resident Vulkan
  projection/visibility/BSP. Ordinary polygons draw unclipped GPU corners;
  supported EX modes use the shared clipping/span stages. Retained input-only
  uploads, palette updates, ordered legacy interleaving and fenced first-eye
  compute are connected. Original/EX Corneria plus sampled Original asteroids
  and EX Titania captures match legacy in both eyes. This is not full migration:
  colour warp, destruction and remaining effect/near-plane
  cases still need work. Existing legacy rejection guards remain, not silently
  bypassed. See `VR-EX-SPAN-MIGRATION.md` for current evidence and limitations.
- Physical Steam Deck input, Android issue #47, Quest and Index acceptance
  remain unproven by the available host tests/captures.
- GitHub recheck: #43/#44/#46/#47/#48 OPEN, #49 CLOSED; no newer updates than
  September 9. No issues were modified during this check.

## Regression refresh (2026-09-11)

- September 12: user requested the grid again. Immersive world assembly now
  restores its grid pass; landscape grids and objects share a single source
  rotation conversion and fixed entry-height camera reference. Vertical
  steering still cannot pitch the landscape. Host packet tests pass. The
  user's apparent colored-ground height issue remains unresolved: the current
  landscape ground is imagery on a surrounding sphere, not a receiver plane.

- User rejected the height-only ground-attachment fix. The follow-up shares
  the landscape rotation with immersive model/shadow projection, instead of
  mixing native camera pitch with background-scroll motion. Per the user's
  newest instruction, vertical steering no longer pitches the landscape at
  all; bank remains interpolated. Tests verify scroll-independent rotation
  and independence from native pitch, while retaining object-local movement.
  Host packet tests pass; physical ground-attachment acceptance remains pending.

- Immersive landscape model/shadow placement now uses the same fixed entry
  camera height as the surrounding ground. Native camera Y steering no longer
  moves stationary objects relative to that ground; object-local vertical
  movement and camera X/Z remain intact. The rule is VR-world-only and does not
  alter non-landscape or ordinary presentation. Host tests cover 13 interpolation
  phases, retained object motion and unchanged non-landscape behavior. Headset
  confirmation of the reported ground detachment remains pending.

- VR Options now includes the desktop's SWAP A/B + Y/X setting, wired to native
  gameplay button mapping (Y/fire swaps with X/boost; A/bomb with B/brake).
  Menu confirmation remains unchanged. Longer pages scroll six readable rows
  instead of reducing font size. Tests cover hold suppression, scroll wrapping,
  input mapping and localized row counts. All 14 host VR tests pass; the host
  VR library and Quest APK build successfully. Installed on Quest 3 using
  install-r, preserving saves; physical gameplay/menu acceptance is pending.
  This does not add the still-unported 2D/3D postprocessing pages.

- VR menu preserves desktop grouping for supported settings: Experience,
  Pace/Speed and MSU-1 Music on Main; Cheats, Crosshair Color, music/SFX volumes
  and Language under Options; the five desktop cheats under Cheats. MSU is
  unavailable without a pack and uses the existing live audio mixer when enabled.
  Crosshair color now updates the captured presentation palette without changing
  native RAM, including EX green. All 13 host VR tests pass (11.08 seconds).
  Desktop 2D/3D enhancement pages are not yet ported to this Vulkan renderer;
  absent postprocessing must not be exposed as functional settings. Settings
  persist across panel reopening, not application restarts. Physical acceptance
  and the reported severe Quest frame stalls remain outstanding.
- VR panel now has a CHEATS submenu: God Mode, Single/Dual/Beam default laser,
  Infinite Bombs, Infinite Boost and Level Select. Settings initialize from the game and
  survive panel reopening. Experience remains locked during play. Input tests
  cover press-only submenu navigation, laser cycling and localized labels;
  all 13 VR tests and the Quest build pass. The shared selected-level API
  configures route state before native stage initialization; tests launch all
  19 Original and 40 EX selectable stages and verify route/stage values.
  The latest APK is installed with persistent mappings, delta uploads, lazy
  pipeline creation and grid removal. Launch remains at the headset's
  controller-required dialog. Physical menu layout, cheat effects and current
  frame-time verification remain outstanding; no performance gain is claimed.

- VR runtime panel wired to Select+Menu press edges. It reuses the visible
  startup panel with RESUME and locked Experience, pauses the frame driver/audio
  output, applies language/god mode changes, and requires button release before
  returning to gameplay. All 13 host VR tests pass (9.70 seconds); physical
  controller/menu acceptance is outstanding. This limited VR panel does not
  expose every desktop F1 option. The headset's previously installed APK
  predates this panel change.

- Quest ARM64 debug APK rebuilt successfully (15 seconds) with all three
  native Vulkan colour-warp pipeline wrappers. The host fixture chains PRNG,
  decode and geometry expansion without intermediate readback; exact outputs
  pass. Live colour-warp arenas/occurrence-indexed graphics are not connected,
  and this build was not installed or verified on a headset.

- Latest ordinary-primitive checkpoint: full host rebuild, all 13 VR tests passed
  (9.47 seconds), and Quest ARM64 debug APK rebuilt successfully (18 seconds).
  Includes resident lines, indexed textures, sprites and the shared simulation
  support for runtime options. No headset installation or physical acceptance.
  Desktop F1 event handling is not a VR-controller menu binding.

- F1 audio source audit found that paused playback still queued emulated audio
  ticks. The host now freezes audio_video_phases/queue_logic_tick while runtime
  options are open and discards overlay navigation audio instead of replaying
  it on resume. Pre-menu pending gameplay audio remains buffered. Windows build
  verification is separate from still-outstanding audible pause/resume testing.

- F1 pause-state regression: Original and EX map/object serialized states remain
  byte-identical over 240 presentation frames and idle menu ticks. Both cartridge
  tests pass (10.70 seconds). RESUME/LOCKED labels now have Japanese, German,
  French and Spanish translations. Interactive audio/input acceptance remains.

- Added requested desktop F1 runtime-options overlay. Bare F1 opens/closes;
  Ctrl+F1 remains save-state and state operations are suppressed while the
  overlay is open. Experience changes and preview-restart requests are locked;
  START GAME becomes RESUME. Flow timing and video phase resume without an intro
  restart. Original/EX runtime-menu tests pass (2/2, 11.11 seconds). Interactive
  desktop verification and equivalent VR menu invocation remain outstanding.

- Indexed-texture checkpoint: Quest ARM64 debug APK rebuilt successfully in
  13 seconds. Resident Vulkan case 14 now checks both interpolated UV axes,
  three palette colours, zero-index transparency, ordered model overlap and
  nearer occlusion. Both eyes pass exact pixel assertions; left capture inspected
  at `tmp/vr-resident-uv-proof/span-depth-14-eye-0.bmp`. No headset install or
  acceptance was performed. Real-cartridge decal comparison remains pending.

- Latest resident-model checkpoint: full `build/vr-dev` rebuild and all 13 VR
  tests passed (10.72 seconds). Quest ARM64 debug APK rebuilt in 13 seconds
  with unclipped ordinary geometry, compact arenas and retained input staging.
  No installation, release, all-level acceptance or physical-headset validation.

- Post-landscape-transition verification: `cmake --build build/vr-dev -j4`
  completed with no pending work; all 13 VR-labelled CTest tests passed in
  10.70 seconds, including both cartridge input/state checks. Quest ARM64
  debug APK rebuilt successfully in 13 seconds with the current sources.
  No installation, publication, or physical-headset verification was performed.

- Landscape transitions now require matching background ID, cartridge,
  atlas origin, unique-half policy and game flow before retaining ground
  height or interpolating tilt. Previously any two landscape scenes could
  share these values, including incompatible Macbeth/Corneria origins.
  Sky and grid use the same compatibility helper; camera discontinuities
  remain additional interpolation cuts. Synthetic mapping-change tests and
  Original/EX state-preservation checks cover the new gate.

- Macbeth BG_3_5 now uses landscape surround with per-cartridge atlas origins:
  Original 272, EX 16. The same origin is passed to sphere sampling and shared
  background/grid tilt. Existing landscapes retain 232. Rear GPU captures
  inspected at `tmp/vr-macbeth-surround-{original,ex}` show the red environment
  without wrapping into the Original atlas's unused turquoise rows. Added
  origin classification and equivalent-scroll-transform regressions. Physical
  headset and full-level transition acceptance remain unverified.

- Macbeth audit found incompatible atlas origins: Original BG_3_5's source
  initializer specifies Y=272, and the live register/source scroll both read
  272. EX's corresponding artwork is shifted up 256 rows and the live scroll
  reads 15 at the sampled tick. It cannot use the hard-coded landscape base
  232. Captures: `tmp/vr-macbeth-scroll-{original,ex}`; next implementation
  must carry a per-background atlas origin through both sphere sampling and
  tilt/grid motion. No Macbeth surround rule enabled yet.

- Fortuna's unique-half GPU payload now requires a 512-pixel atlas made of
  8-pixel tiles. Narrow and oversized-tile fixtures are rejected without
  replacing the retained scene; live Fortuna rear capture still passes.
  Quest ARM64 debug APK rebuilt successfully (32 seconds), including the
  latest Venom/Fortuna surroundings and shutter work. Not installed or
  accepted on a physical headset.

- Fortuna BG_3_3A surround is now enabled for Original/EX. Outside the primary
  forward 256-pixel interval, the GPU samples the moon-free right atlas half;
  sky/cloud/ground repeat without multiplying the moon. Four Original compass
  captures inspected in `tmp/vr-fortuna-surround-{0..3}`: one forward moon,
  none behind or on either side. EX rear capture passed in
  `tmp/vr-ex-fortuna-surround`. Packet tests reject a narrow atlas for this
  rule and preserve shared sphere geometry; cartridge classification/state
  tests pass for both variants (3/3 targeted tests, 10.89 seconds).

- Fortuna surround audit: Original/EX BG_3_3A decoded atlases are identical
  (`93098AF5FA0EC93D6024A4E0AB3F62597B3E4F1A90197DCCAF6CF464EE55BE7F`).
  The left/right 256-pixel halves differ only at 1,374 pixels inside
  x=88..143, y=265..295: the single moon. Thus outside the primary forward
  occurrence, the right atlas half provides an exact moon-free cloud/sky/ground
  repeat. Captures are `tmp/vr-{ex-,}fortuna-atlas`. Fortuna is not yet enabled
  as a landscape surround; it needs this unique-object sampling rule first.

- Venom route 3's BG_3_7A joins the outdoor surround mapping. Four decoded
  atlases (routes 1/3, Original/EX) are byte-identical, SHA-256
  `853FAFCC0025A885583619D8B16E31F10D7EFC66CCE4E2D99D704A72A5326BA8`.
  Source initializers also share F1 artwork, palette 2A and Y=232 origin.
  Route 2's Mode-1 tunnel backgrounds remain excluded. Added both-route
  classification coverage; route-3 rear GPU capture passes in
  `tmp/vr-venom3-surround`.

- Original/EX Venom outdoor BG_1_6A now uses the landscape surround. Both
  captured atlases match the source's Y=232 outdoor layout; Mode-1 tunnels
  and BG_1_6C remain outside this rule. Original front/rear renders inspected
  in `tmp/vr-venom-surround-{0,1}`; EX rear capture passed in
  `tmp/vr-ex-venom-surround`. Original/EX classification and complete-state
  preservation regressions pass (2/2, 9.51 seconds). No headset verification.

- Save continuation harness now optionally compares final GPU presentation
  sequences (`-Presentation`), requires the resident path and rejects transition
  readback/replay. First fresh-process Original fixture (600 preroll ticks)
  matched its 30 compared presentation frames but failed the existing audible
  PCM requirement because all blocks were silent; not counted as a full pass.
  A subsequent launch was blocked by Windows as potentially unwanted software.
  No exclusion, protection change, relocated binary or launch bypass was used.
  Desktop runtime verification must resume only after that external block is
  resolved; source work and unrelated verification remain available.

- Live VR shutter captures now exercise the cartridge's MSCRAMWIPE_CIRCLE
  command over Original and EX Corneria geometry, backgrounds and HUD.
  `--live-stage=LEVEL1_1:scramble` selects a bounded ten-tick source fixture.
  Inspected `tmp/vr-live-shutter-{original,ex}/live-scene-left.bmp`: straight
  black upper/lower margins cover the full viewport, leaving the live scene
  visible only inside the opening. Both stereo capture runs pass. This is
  host Vulkan evidence; physical headset acceptance remains outstanding.

- VR shutter GPU proof now passes eight readbacks: closed, partial, completing
  and released, each in both eyes. Every RGB pixel matches independently
  calculated opening bounds against a white clear target. Captures:
  `tmp/vr-shutter-gpu-proof/shutter-{0..3}-eye-{0..1}.bmp`; partial/completing
  images are straight full-width bands. This proves the primitive/pipeline
  coverage, not headset comfort or the complete live-game transition.

- VR now snapshots the source window wipe and submits recognized horizontal
  scramble shutters as two black GPU quads over the complete eye viewport.
  The mask is head-locked, drawn after HUD without depth testing, and uses the
  shared final-opening interpolation. Packet tests cover closed, partial,
  completing and unrelated window programs; actual headset/GPU image proof
  for this newly added pass is still required. Snapshot state-preservation
  tests caught bus side effects from `window_wipe_state`; its RAM reads now
  use non-mutating peek access instead of native bus reads.

- Complete 240-FPS SBS wipe capture exposed a final-step defect missed by the
  earlier one-second fixture: source mask deactivation jumped at frame 272.
  Recognized, partially open horizontal shutters now interpolate to the full
  192-row opening before releasing the mask; unrelated completion cuts remain
  unchanged. Added simulation regressions and `tools/check_sbs_wipe.ps1`.
  Original Full SBS completes at frame 291 with 117 sampled edge positions,
  matching eye bounds and no edge step above two output pixels in 480 frames.
  Evidence: `tmp/sbs-wipe-regression` (before: `tmp/sbs-wipe-complete-240`).

- SBS frame-sequence capture now works for both Half and Full output, not just
  mono. The harness verifies fresh files and dimensions for every requested
  frame and rejects profiling with readback enabled. EX Half/Full each passed
  24-frame captures. Original Full SBS passed a 240-frame, 240-FPS injected
  scramble-wipe capture in `tmp/sbs-wipe-sequence-240-full`, with GPU wipe trace
  and no stereo fallback. Sampled nonblack row bounds agree between eyes on
  every frame; after the injected command closes the image, the opening grows
  from rows 111..112 to 24..199. This one-second fixture does not cover the
  entire opening duration or prove all source transitions.

- Water interpolation correction: the VR tile vertex shader now applies the
  receiver's interpolated Y scale to the fragment-local inverse projection.
  Previously geometry moved but texture sampling retained the source-tick
  height. `tools/check_vr_water_height.ps1` compares identical world planes
  encoded as native-height geometry versus half-height geometry scaled 2x.
  Captures live under `tmp/vr-water-height-regression`; this checks texture
  projection equivalence, not full moving-headset or scanline-timing parity.

- Longer world sweep: all 59 numbered stages (19 Original, 40 EX) passed
  2,400 ticks each with cached/uncached geometry and transforms compared at
  alpha 0, 0.5 and 1. No unsupported packet or cache mismatch in these paths.
  This does not exercise every object, gameplay branch or EX effect.
  Preflight now reports explicit per-effect object-tick flag counts, including
  zeroes, to distinguish an unexercised effect from verified support.

- Quest ARM64 debug APK rebuilt successfully after enabling EX Titania's
  outdoor surround (Gradle 10 seconds). Not installed or headset-verified.
  EX LEVEL1_1 extended preflight passed 2,400 logic ticks / 159,807 packets
  with three interpolation samples per frame; this is geometry assembly,
  not a rendered-effects acceptance test.

- EX Titania outdoor BG_2_3A now uses the landscape surround, matching the
  Original projection layout instead of leaving a flat background panel.
  Decoded atlas comparison differs at 2,900/262,144 pixels but preserves the
  sky/cloud/ground layout; front and rear Vulkan captures inspected in
  `tmp/vr-ex-titania-outdoor-surround` and its `-rear` counterpart.
  Full vr-dev build and 13/13 VR regressions PASS (10.12 seconds).
  Original/EX classification tests require this landscape independently of
  the menu experience choice. These are host captures, not headset acceptance.

- After DXR resident-output and exact-geometry acceleration-structure caching:
  full Windows desktop build PASS; 54/54 desktop CTests PASS (133.09 seconds).
  Full vr-dev build PASS; 13/13 VR-labelled tests PASS (11.17 seconds).
  All processes completed. These are host regressions, not physical headset
  acceptance or proof that the explicit migration gaps above are resolved.

- Original ending fixture (3000 preroll ticks, 1200 frames) now provides actual
  GPU projected-text coverage with no stereo fallback in FULL and HALF SBS.
  Inspected `tmp/sbs-original-ending-text/presentation.bmp` (800x224) and
  `tmp/sbs-original-ending-half-text/presentation.bmp` (400x224): readable paired
  producer credits. EX at the same timing does not hit projected text; its
  coverage gate failed and is not counted as proof. Added Ending harness mode
  and the projected-text shader to CMake's generated-source freshness checks.

- Added LevelClear capture mode and pinned capture timing to 60 FPS/source
  timing. Original LEVEL1_2 clear captures (1200 and 2400 frames) completed
  without stereo fallback but did not exercise projected text; RequireText
  correctly rejected them. Stage-tally screenshots are not proof of the native
  projected-message path. MAIN.ASM's makenumt/makenump create final total-score
  text, so the ending fixture is the next source-grounded coverage candidate.

- Capture harness now accepts explicit preroll ticks and RequireText, rejecting
  captures that never record a projected text object. EX TITLEMAP (100 ticks)
  and INTROMAP (100 ticks, 300 frames) rendered but failed that coverage gate;
  neither is text-migration proof. Inspected the fresh intro SBS image: paired
  intro nebula/planet/message scene, not the projected-message object path.
  Source call sites are PATHS.ASM's text-object path and MAIN.ASM's text creation
  routines; use these to select the next meaningful runtime fixture.

- Stereo projected text now passes exact near/convergence/far pixel checks for
  both independent GPU eye outputs at 1x/2x/4x, including zero disparity at the
  convergence plane. Full projection diagnostic passes. Added an opt-in GPU
  trace marker for proving that subsequent game captures exercise this path.

- Projected ROM text now snapshots glyph rows and uses a two-pass GPU renderer
  in native GPU gameplay/SBS scenes. Stage one projects once; stage two emits
  packed pixels, retaining transparent glyph holes and ordered composition.
  Menu/HUD text is unchanged. Vulkan/Metal generation, Windows game build and
  GPU diagnostic pass, including direct versus scene text pixels at 1x/2x/4x.
  Stereo text disparity and photographic gameplay coverage remain unverified.

- Replaced particle trail per-row full Bresenham walks with integer row bounds:
  shallow trails use binary search and steep trails direct minor-axis rounding.
  Regenerated SPIR-V/Metal; full GPU diagnostic passes, including explicit
  all-octant/zero-length/half-step cases and 1x/2x/4x final-pixel comparisons.
  This establishes pixel parity, not an in-game FPS improvement measurement.
- Prior particle integration VR build passed; 13/13 VR tests passed in 20.29s.
  Added particle snapshot replay/stereo isolation tests and fixed inactive-only
  mixed scenes to return cleared output (GPU readback verified), not no output.

- Integrated particle snapshots into ordered GPU scenes, stereo eye projection,
  recording/replay and the native GPU gameplay branch. Controls-only alternate
  targets retain CPU rendering. Alternating particle fixtures now exercise the
  scene upload/projection/span/raster/merge path with exact final-pixel parity.
  Windows game build, full GPU projection diagnostic and SBS unit tests pass.
  Gameplay photographic coverage and particle trail performance remain pending.

- Added owned particle snapshot uploads with cycled transfer/storage buffers;
  filtering, palette wrapping and source-word data are preserved while GPU
  interpolation/projection remains unchanged. Alternating raw/snapshot fixtures
  passed the 4,500-particle projection and final-pixel checks at 1x/2x/4x.
  Full projection diagnostic passed. Scene/runtime particle integration remains
  pending; this does not establish hardware VR completion.

All 15 particle batches now compare final packed raster pixels against CPU
dots/Bresenham trails at 1x/2x/4x, including clip columns and overlaps: pass.
Shader skips trail walks for rows outside endpoint bounds; regenerated portable
code and reran full GPU diagnostic successfully. Further trail optimization,
owner snapshot uploads and game integration remain pending.

Added resident particle span dispatch to shared point shader: 2x2 dots,
Bresenham trails, effect clip and per-particle palette. SPIR-V/Metal regenerated;
dispatch executes across existing particle batches and GPU regressions pass.
New span output is not pixel-parity-verified yet. Current trail implementation
walks the segment per row and needs performance review before live integration.

Expanded particle GPU projection parity to 4,500 particles: 15 full batches,
five interpolation alphas, signed-word wrap fixtures, near-plane trail gating,
fractional owner offsets, varied widths, mono/left/right eye offsets. All eight
output fields match CPU reference; complete existing GPU suite passes. Particle
pixel/trail rasterization and game scene integration are still pending.

Added validated caller-command particle GPU dispatch and device cleanup.
An interpolated trail fixture verifies both endpoint coordinates, palette and
trail visibility exactly on GPU: pass. Existing full GPU diagnostic passes.
This is a smoke test only; varied particles/word wrapping/stereo parity,
trail span rasterization and scene integration remain pending.

Added portable particle projection shader: source-word interpolation, binary64
owner translation/projection, stereo offset, palette/trail flags and both-endpoint
visibility. Strict SPIR-V/Metal generation and freshness checks pass. No GPU
dispatch/parity proof yet; host input validation/upload, span generation and
scene integration remain pending. Existing particle gameplay path unchanged.

Added owned per-owner particle snapshots and a shared span-based CPU renderer
for immediate drawing/deferred replay, preserving filtering/order without
adding allocations to the immediate path. Dot, interpolated trail and effect
clip fixtures pass. GPU particle projection/span generation and scene/runtime
integration are not implemented yet.

Original CONTMAP Full-SBS capture at tmp/sbs-controls-dust/presentation.bmp
passes required GPU-dust trace, dimensions and no-fallback gates; image inspected
with stars in both small ship viewports. Desktop VR build passes; all 13 VR
tests pass in 20.29 seconds after dust integration. Motion/headset parity is
not established. Next remaining CPU geometry reviewed: owner particles use
wrapped interpolation, near-plane/edge gates, clipped 2x2 dots or Bresenham
trails interleaved with models; they still require GPU conversion.

Added capture harness -Level and -RequireDust, with trace gate for actual GPU
dust recording. Original LEVEL1_2 Full-SBS and EX LEVEL1_2 Half-SBS captures
pass dimensions/submission/no-fallback checks; both images inspected with
visible stars. Files: tmp/sbs-space-dust-original/presentation.bmp and
tmp/sbs-space-dust-ex/presentation.bmp. Dust GPU final-pixel diagnostic now
also traverses GpuScene for one wrapped fixture: pass. Controls, temporal and
cross-device verification remain; these captures are not full completion.

Integrated GpuDustDraw into ordered scene recording, GPU upload/projection/
spans/raster and scaled CPU fallback. Desktop dust now records immutable source
points when native models/SBS are enabled, preserving controls viewport offsets.
Eye copies receive independent projection offsets without changing source data.
Desktop build and stereo/replay tests pass. Space-level and controls captures,
GPU scene dust pixel comparison, temporal/device checks remain required.

Added enqueue_dust_frame: source snapshot packing, cycled point/palette upload,
matrix/viewport settings and resident projection on the caller's command.
Three upload/wrap/buffer-reuse fixtures match CPU final palette pixels; the
full dust/grid/model diagnostic passes. Deferred scene draw integration and
real gameplay/controls-screen capture remain pending.

Dust final-pixel fixtures now run at 1x/2x/4x (including exclusion and near
companions): pass. Added source-relative binary64 input packing with immutable
snapshot coordinates and finite-camera/count validation. Fractional signed-word
wrap and invalid-camera tests pass, alongside the shape/GPU suites. Actual
scene upload/recording and gameplay capture remain pending.

Dust projection now feeds GPU row spans and raster output without CPU readback
between stages. Shared point-span shader supports dust palette/companion flags
and per-pixel exclusion. All 18 dust fixtures now compare final packed pixels
against CPU output, including overlapping stars and excluded columns: pass.
Existing grid/line/model projection suite passes. Scaled dust, input packing
and actual game recording/submission remain pending.

Expanded dust GPU parity to 18 full 511-point batches (9,198 records), checking
all output fields against CPU matrix/projection arithmetic. Includes fractional
coordinates, explicit near/companion/clamp depth boundaries, distinct palette
banks, viewport offsets and both eye offsets: pass. Existing GPU suite passes.
Input wrapping, final dust pixel emission/exclusion and gameplay integration
still need implementation/verification; this is not full dust migration proof.

Added GpuProjection::enqueue_dust with validated dimensions/count/eye settings,
lazy portable pipeline and resident output. Built and executed a one-point GPU
smoke fixture verifying centre coordinates, palette and near-companion flag.
Complete existing GPU projection/grid suite passes. This is only a smoke test;
broad dust arithmetic/palette/viewport parity and runtime integration remain.

Added dust_portable shader for binary64 matrix/projection, source depth clamp,
palette ordering, companion flag and stereo disparity from wrapped-relative
input coordinates. SPIR-V/Metal generation, freshness and binding tests pass.
Shader has not been dispatched or parity-tested; CPU packing, GPU host wiring,
span generation, exclusion handling and gameplay integration remain pending.

Added owned DustFrame snapshots (source world points, palette, camera, matrix,
viewport offsets/exclusion) and CPU replay for deferred rendering. Snapshot
replay matches existing controls-viewport dust at three widths; shape tests
pass. Existing immediate CPU path is unchanged. GPU dust shader/upload/scene
integration is not implemented yet; this establishes immutable input/fallback.

Added capture-harness -Profile with explicit 60-frame warmup, >60 frame guard,
required timing output and isolation from inherited profiling flags. EX Full-SBS
300-frame connected-grid samples summed 7,879 and 7,818 microseconds across
background/world/composite/present stages; dot-grid sample summed 6,940 us.
These are local CPU stage timings, not GPU timestamps or an optimization A/B
claim (lines and dots are different visuals). Repeat capture passes with no
stereo fallback. Dust review: up to 511 world-space points, wrapped relative
coordinates, 4095 projection-depth clamp, indexed depth palette and close-point
companion pixels all need preservation in the next GPU conversion.

Added explicit test-only EX grid-line selection and capture-harness -GridLines
gate requiring the actual GPU recording trace. Desktop rebuild passes; EX
Full-SBS capture tmp/sbs-ex-connected-grid/presentation.bmp passes dimensions
and submission checks with no stereo fallback. Inspected both views: connected
lines are visible. This proves the path is exercised, not all temporal or
stereo-depth behavior. Benchmark and broader scene/device checks remain.

Connected-grid mode now records GpuGridDraw with a shared source-frame start,
dispatches 675 GPU span blocks and replays through the extracted CPU line
renderer on fallback. Desktop build, shape tests, stereo tests pass; new replay
test checks scaled line pixels and preservation of history across eye copies.
Needs an EX line-enabled gameplay capture, timing and temporal stereo checks;
the current captures only establish dot-grid rendering.

Replaced connected-grid per-row full line walks with bounded binary searches
over exact integer source Y offsets. Expanded final-pixel checks to 32 lattice
fixtures with varied slopes and signed-word carried endpoints: all pass, as
does the complete GPU projection diagnostic. No gameplay FPS improvement
claimed without timing; connected-line runtime integration remains pending.

Added connected-line mode to portable grid span generation: marker, asymmetric
source line walk and near companion use separate row-span blocks. SPIR-V and
Metal regenerated; one 64x64 final-pixel line fixture matches CPU exactly.
Existing 24,300 dot projections/scaled pixels and GPU regression suite pass.
Line mode is not wired into gameplay yet. Broader line fixtures, performance
work (current shader walks the segment per row), and stereo integration remain.

Extracted DustRenderer::prepare_grid_lines to snapshot projected source points
and the carried endpoint independently of pixel drawing. CPU line rendering
now consumes that snapshot. Added tests showing repeated/interpolated
presentations retain the frame start and cannot overwrite the endpoint carried
to the next source frame. Shape decoder suite passes. GPU connected-line span
generation and runtime stereo line integration still remain to implement.

Cross-build refresh after grid integration: desktop VR build passes; all
13 VR tests pass (20.51 seconds, including Original/EX game-input fixtures).
Quest arm64 debug APK rebuilt successfully in 19 seconds, not installed or
headset-tested. Explicit GPU stale-command, mismatched-height and failed
projection guard tests pass. Connected-line review confirms carried endpoint
history must remain source-frame-owned across interpolated presentations/eyes;
do not advance it independently for each stereo eye.

Hardened grid spans against stale/mismatched projected data: require the same
command and matching logical height; a failed projection invalidates reuse.
Added CPU replay scaling and immutable stereo-grid eye-offset tests: pass.
Full GPU projection diagnostic still passes. Rebuilt desktop and captured EX
Half-SBS with bloom at tmp/sbs-grid-ex-half/presentation.bmp: submission and
dimensions pass; image inspected. Temporal/device-wide parity remains open.

Connected GpuGridDraw to ordered scene recording, GPU projection/span/raster
dispatch, device cleanup, stereo eye offsets and mono CPU replay. Desktop
dot-grid path now records geometry when native models/SBS are enabled.
Desktop build and stereo diagnostics pass. Original Full-SBS capture at
tmp/sbs-grid-integrated/presentation.bmp passes submission/dimension checks;
visually inspected both views with ground dots present. This is not proof of
all-level or temporal parity. Connected grid lines and dust remain CPU raster
chunks, and scaled/EX/runtime-fallback regression checks remain outstanding.

Extended grid diagnostic through resident GpuRaster, checking complete packed
pixel output (palette/tag/coverage) and all row-command fields at 1x/2x/4x.
24,300 mono/stereo point records across random and explicit near-plane,
viewport-boundary, companion-dot and depth-clamp fixtures pass exactly.
GPU projection regression suite passes. Runtime grid recording and connected
grid-line handling remain outstanding; this does not establish game parity.

Added portable grid row-span generation and resident enqueue_grid_spans.
SPIR-V/Metal generation and freshness validation pass. GPU diagnostic now
checks all 24 words in 4,838,400 generated row commands against CPU dot rules
(96 mono/stereo lattice cases, 225 points, 224 rows), including empty rows and
near-depth companion pixels: pass. Existing projection suite also passes.
This is not yet runtime integration; scaled-grid coverage and final raster
image comparison remain required before replacing the current grid drawing.

Added GpuProjection::enqueue_grid: caller-owned command dispatch, resident
225-point output, lazy portable pipeline, validated dimensions/stereo inputs,
and device cleanup. GPU diagnostic passes 21,600 mono/left/right grid records
across wrapped random lattices exactly against the CPU arithmetic reference.
Existing native/continuous/axis GPU diagnostic checks also pass. Runtime
per-eye grid raster emission is still pending; no visual completion claim.

Registered grid_portable in the offline shader generator and CMake freshness
gate. Generated SPIR-V and Metal successfully; freshness check, six Metal
binding tests and three source-digest tests pass. Rebuilt and ran the shape
decoder tests (including existing source-grid projection coverage): pass.
This is shader/build integration only: GPU dispatch, CPU/GPU grid parity,
per-eye raster emission, and runtime integration remain outstanding.

Started native grid compute projection in shaders/grid_portable.hlsl: source
15x15 lattice, signed-word wrapping/reciprocal projection, and per-eye shift
before viewport clipping. DXC Vulkan SPIR-V compilation passes. Not registered
with the portable generator or runtime yet; Metal generation, GPU/reference
readback and raster emission remain. Do not claim grid migration is complete.

Confirmed EX harness token is recognized by runtime's exact EX selector and
test runs do not persist pregame/EX save changes. Started grid stereo migration
by exposing compact source_grid_lattice (camera-space origin and wrapped X/Z
steps) before screen clipping. project_source_grid consumes the same helper;
core builds. This preserves offscreen lattice geometry for future eye frusta,
but does not yet render grid depth in SBS. Grid parity regression refresh needed.

Added tools/check_sbs_presentation.ps1 for repeatable Original/EX Half/Full
captures with configurable bloom. It checks process exit, fresh BMP, exact
16:9 output dimensions and stereo trace without fallback; timeout reports the
live PID instead of restarting/killing it. EX Half + medium bloom passes and
was visually inspected at tmp/sbs-harness-ex-half/presentation.bmp. The harness
explicitly does not claim depth or complete gameplay parity.

Two-pass SBS validation: build 11942 passes. Original Full SBS repeat at
tmp/sbs-game-full-two-pass.bmp is SHA256-identical to prior three-pass capture
tmp/sbs-game-full.bmp: ED74D969A5D177BE9975C29B5CCEA1A210CC962E4C60276AE5B98A25046532A4.
Log confirms 800x224 presentation. This proves no output change in this fixture,
not a measured FPS gain. No running process remains.

Original/EX state continuation refresh 4485 passes both tests in 27.59 seconds.
SBS scene submission now attempts the stereo pair at the recorded-scene stage
and reuses it at presentation, eliminating the unconditional preliminary mono
model pass. Mono is submitted only for failure fallback; if that submission
fails too, force the complete recorded CPU replay instead of stale GPU output.
Readiness resets for native raster frames and resource release. Build 98680
passes before the last readiness-reset edit; follow-up build is running.
Must repeat live capture and benchmark before claiming measured speedup.

Fixed state-load SBS reset: restored_state now preserves the current session's
stereo_output without changing archive bytes/version. Added cartridge-state
regression asserting Full SBS survives restore and serialized timeline bytes
remain identical. State test target builds; Original/EX full continuation
tests are live in session 4485. Poll this handle before starting another run.

Corrected window presentation aspect: Full SBS uses twice the mono logical
display width; Half retains mono aspect. Ordinary mono presentation restores
its logical dimensions after SBS so toggling OFF cannot retain a double-wide
letterbox. Source 4:3 pixel-aspect correction uses existing presentation_width.
Window-size/interactive toggle checks remain pending; raw eye captures retain
their raster dimensions. World dust/grid and particles are confirmed pre-rasterized
in starfox_pc before model draws; they need stereo-aware geometry submission.

Additional live SBS captures pass presentation and were visually inspected:
Original Half SBS + medium bloom -> tmp/sbs-game-half-bloom.bmp (400x224);
EX Full SBS + medium bloom -> tmp/sbs-ex-full-bloom.bmp (800x224). Both eyes
show scene and glow; runtime logs record expected dimensions. No crashes.
These short LEVEL1_1 fixtures do not prove all gameplay, world raster depth,
ray-traced stereo shadows, aspect correction or every effects combination.

Full SBS live capture now succeeds: black image was an early mono/loading
capture consuming the shared counter (299x224), not the final stereo target.
Added dedicated stereo capture counter; build 36187 passes. Repeated Original
LEVEL1_1 48-frame capture produces 800x224 at tmp/sbs-game-full.bmp, visually
inspected with both game views visible. Log confirms stereo presented 800x224.
This supersedes the black-capture conclusion below. Half SBS, EX, detailed
disparity/world elements, effects and menu layout still need verification/fixes.

First desktop Full-SBS capture (Original LEVEL1_1, 48 frames, preroll 1000,
GPU renderer) is BLACK and appears mono-sized; runtime log reports native model
batch/composition but no stereo-specific evidence. Capture inspected at
tmp/sbs-game-full.bmp, log tmp/sbs-game-full.log. Do not claim live SBS works.
Added stage-specific stereo submission/effects/retention diagnostics and a
successful-presentation size trace; rebuilding before rerunning same capture.

Desktop SBS frame-loop connection implemented: nonzero selection records model
draws, submits eye scenes, applies composition/effects separately, retains each
eye, and packs into a GPU target (Full doubles width). Mono fallback remains
on unsupported/failing frames. Build 64008 passes. Added stereo capture support
and STARFOX_TEST_STEREO_OUTPUT override; follow-up build launched for these.
Live capture still required; world raster chunks remain mono and must be fixed.
This is development integration, not finished/verified SBS or migration.

Window now retains each finished eye in a separate GPU render-target texture,
compositing base, separated model and bloom layers. Retention uses high-resolution
model target dimensions when enabled and flushes source texture reads before
the next eye reuses effect outputs. Stereo textures release on renderer switch
and destruction. Build 44497 was launched; this helper is not yet called by
the frame loop, so live stereo behavior remains unverified.

Desktop native compositor/effects entry now accepts an explicit eye output and
can defer display, returning success/failure. Deferred eyes never call a mono
fallback or present between eyes; ordinary mono callers retain their existing
fallback. Desktop build 43820 passes. Next: retain each finished eye's textures,
compose split bloom/model layers, then invoke packing from the actual frame loop.

Desktop Window now owns/releases a native stereo scene during destruction and
renderer switching, with a submit_stereo_scene entry point. GpuStereoScene adds
owned submissions using independent per-eye fences/generations and exposes no
partial pair on failure. Desktop build 29245 passes. GPU diagnostic also passes
owned-pair readback at convergence (exact CPU pixels) plus borrowed submissions
at other depths. The Window entry point is not yet called by the frame loop;
per-eye composition/effects and final packing still need connecting.

GPU packing diagnostic now passes actual texture readback: distinct red/blue
eyes pack to 8x4 OFF, 8x4 Half SBS and 16x4 Full SBS with every RGBA pixel exact.
This catches swapped/cleared eye halves and destination extent mistakes; solid
colors do not prove detailed-image filtering quality. Native stereo scene
tests still pass four paired frames with exact CPU pixels and expected disparity.
All diagnostic jobs terminal. Runtime connection remains the next unfinished step.

Added GPU-only final eye-texture packing using SDL blits: Half uses linear
horizontal downsampling into normal width; Full uses full-width eye regions
in a doubled-width target. First pass clears, second LOADs without cycling so
left-eye pixels survive. Aliased eye/destination handles are rejected. Build
and existing stereo CTest pass; packing itself still needs GPU readback coverage
and runtime connection. No CPU readback belongs in the final presentation path.

Native stereo diagnostic failure resolved: the synthetic triangle used reverse
source winding; CPU also rendered zero. Correct cartridge-compatible winding
renders matching coverage (5,405 / 1,455 / 389 pixels at depths 100/200/400).
Both eye outputs survive paired submission; repeated depth 100 reuses storage
successfully. Disparities 8/0/-4 match expected 8.192/0/-4.096 within native
pixel quantization. Added exact per-pixel CPU comparisons for both eyes;
rebuilt diagnostic passes all four paired frames. No shader workaround used.
Desktop presentation and raster-world stereo remain pending.

Native stereo GPU diagnostic added as `starfox_gpu_stereo_check` and builds.
Execution currently FAILS at nonzero coverage assertion (line 49): synthetic
triangle produces too few/no colored pixels. Explicit forced colour did not
resolve it. Must compare mono/CPU fixture rendering and inspect per-eye output
before attributing this to the submission wrapper. No process remains active.
Do not treat the new native wrapper as GPU-verified; earlier Vulkan diagnostic
is distinct. The diagnostic uses both eye downloads after both draws to catch
output reuse and checks several depths plus repeated invocation once nonblank.

Added native GPU stereo-scene submission: eye-specific copies of ordered model
draws use parallel camera translations/off-axis vanishing points; two GpuScene
instances retain both outputs. Both eyes encode into one caller-owned command,
which must be cancelled in full on failure. Raster chunks remain shared and
source animation/lighting remain untouched. Stereo unit tests pass ordering,
source immutability, offsets, convergence and invalid input. Actual GPU use of
this new native submission wrapper is not yet tested; the earlier Vulkan SBS
camera test exercises a different backend. Desktop presentation/packing remains
unconnected. World raster primitives still need stereo-aware handling rather
than treating every pre-rasterized chunk as a convergence-plane UI layer.

SBS development menu now exposes OFF/HALF SBS/FULL SBS at Options row 9
(top visual row, existing slider coordinates unchanged). GameSimulation owns
the selection, cycles left/right/confirm, and desktop load/capture persists it.
Added Japanese/German/French/Spanish labels and regenerated the menu catalog.
Desktop/simulation build 59997 passes before the catalog regeneration; next
desktop build must pick up those translations. Menu regression session 56891
was launched. Crucially, selection does NOT yet switch the renderer: output
integration is pending, and this development UI must not ship as functional SBS.

Quest refresh 81700 passes (41 seconds), APK at
platform/quest/build/outputs/apk/debug/quest-debug.apk; not installed.
New Vulkan SBS diagnostic `starfox_vr_scene_check tmp/sbs-camera-proof --sbs`
builds and passes on Intel Graphics: 1,627 green pixels in each eye, centroids
128.714 / 126.286, disparity matching the independent off-axis expectation.
Both BMPs were visually inspected: near green triangle correctly occludes
far red triangle. These are synthetic camera proofs, not game SBS output.
All jobs in this checkpoint are terminal; desktop runtime integration remains.

Latest full validation is terminal and green: desktop CTest 66006 passes
54/54 in 165.19 seconds; full VR build 48273 passes; VR CTest 13803 passes
13/13 in 10.40 seconds. These supersede the older live-job notes below.
Neither suite proves SBS runtime integration or device visual parity.
Quest APK refresh is now live in session 81700 using the workspace JDK/SDK.
Poll that exact handle; no headset installation was requested or performed.

Current verified live jobs: desktop CTest 66006 (54 tests; latest completed
source state/level-clear checks pass), full VR build 48273 (core/game libraries
and application tests linked; desktop/VR final targets still pending). Poll
these handles, do not restart. Previous goal turn was progress; this turn also
refreshes cross-target build evidence.

SBS integration inspection: desktop `GpuSceneRecording` interleaves model
draws and already-rasterized layer chunks, while `GameSceneHistory` owns the
source snapshot needed for both Vulkan eyes. The new EyeCamera helper alone
does not connect these pipelines. Preserve painter/layer ordering, source
lighting and one simulation advance per stereo pair when wiring output.

Desktop full build 52654 passes, including the changed settings capture.
Added headset-free SBS camera construction to the existing Vulkan EyeCamera
interface, using parallel views and asymmetric projection matrices. VR camera
target rebuilt; executable passes convergence, opposite near/far disparity,
equal vertical/depth coordinates, invalid-input checks and existing VR tests.
This does not yet connect the desktop draw loop to the stereo cameras.
Full desktop CTest refresh is live in session 66006; poll that exact handle.

SBS persistence now uses settings revision 13 and `STEREO_OUTPUT` values 0/1/2.
Runtime settings capture preserves this field while renderer integration is
pending. Dedicated input tests pass all three round-trips, reject invalid
values without mutating loaded settings, and verify V12 files default OFF.
Stereo projection/layout and runtime-input CTests both pass. The changed
desktop capture lambda still needs the next full desktop build. No menu or
runtime SBS output is exposed yet; this is not a completion claim.

Both backface catalogs are now terminal and pass: Original 2,697 models /
48,546 images and EX 3,511 models / 63,198 images, zero deferred models.
All 111,744 comparisons have exact coverage/palettes; max normal error
1.19209e-7, depth .000244141. EX nonzero pixels: 519,808,698.
No catalog process remains active. This does not enable native GPU gameplay.

SBS implementation started with independent output-layout and parallel,
off-axis eye-projection helpers (`render/stereo_output.hpp`). The dedicated
CTest passes Half/Full sizing, unchanged scene aspect, viewport coverage,
overflow/invalid input rejection, convergence-plane agreement and opposite
near/far disparity. This is foundational math, not yet a working menu or
runtime stereo renderer: those integrations and photographic proof remain.

Confirmed user scope: one `3D OUTPUT: OFF / HALF SBS / FULL SBS` setting under
main-game Options. Half packs distinct left/right eye views into the normal
frame width; Full uses twice the horizontal resolution. A duplicated flat
image is not stereo output. This requirement is not yet implemented/verified.

Latest backface catalog refresh: Original scan 52935 passes all 2,697 models /
48,546 images / 342,299,892 nonzero pixels with zero deferred models and exact
coverage/palettes; maximum normal error 1.19209e-7, depth .000244141.
EX scan 55278 remains active as of this checkpoint. Earlier scan statuses below
are historical and superseded. These diagnostic checks are not game-FPS results.

SHIP_0_C viewport precision fix passes: raw-camera polygons retain binary64
XY/UV through all four viewport planes, matching source interpolation (without
forcing the clipped axis to an exact boundary). Shader generation 90192 and
checker builds pass. Host quarter-pixel regression passes; GPU clipping suite
passes all fifteen fixtures. SHIP_0_C whole-model passes 18 images / 3,609,797
pixels, exact coverage/palette, normal error 5.96046e-8, depth .0000305176.
Both prior catalog scans stopped at this shared case; they must be rerun.
No build/test processes remain active at this checkpoint.

Repeat whole-model MBLACKFACE passes 18 images / 3,119,452 pixels; BEEANIM
repeat passes 18 images. CPU-left/GPU-right proof images under
tmp/mblack-fixed-proof and tmp/bee-fixed-proof were inspected and match.
Full Original backface scan 66879 is terminal at a new failure: SHIP_0_C
view 0/scale 2, coverage pixel 42857. EX scan 5401 remains live. Do not call
the full backface catalogs complete.

Matrix-path fix verified: backface-enabled continuous draws now retain camera
residuals even for matrix poses; format 3 packs reconstructed exact camera
values when Euler exactCamera is not present. Shader regeneration and build
33414 pass. MBLACKFACE isolated faces pass 72 images / 3,127,161 pixels,
coverage/palette exact and zero normal/depth error. BEEANIM whole-model passes
18 images / 366,320 pixels, exact coverage/palette, normal error 5.96046e-8,
depth error .0000610352. Their 4x tangent failures are resolved in these checks.
Full Original/EX backface catalogs launched next; catalog completion unproven.

Compiler memory fix: O1 job 37316 also grew past 24 GB private memory and was
stopped deliberately. A noinline probe preserved 2 functions/2 calls in SPIR-V.
Marked shared sf_add/sf_mul/sf_div [noinline] in HLSL only (host arithmetic
unchanged). Full default-optimization strict-IEEE shader generation now passes
for clip/continuous/axis in seconds, not the former memory-exhausting run.
GPU arithmetic probe passes 262,144 exact-bit results. All generation jobs and
checker build 30711 are terminal. No lower optimization level was adopted.

Updated clipping suite passes all fifteen residual fixtures, 25,362 native
polygons and 48,435,200 fractional span pixels. MBLACKFACE isolated scan now
gets through face 2 (including formerly culled view 4) and stops at face 3,
view 1/scale 4 pixel 448. Whole-model completion is still not claimed.

Resource safety: stopped DXC process 15492/session 27095 deliberately after
measuring 60,350,722,048 bytes private memory and only ~1.4 GB system commit
remaining. This was not an observation timeout or a spontaneous compile
failure. Session is terminal (exit 1 from terminated child); no other apps
were stopped. Testing lower optimization (-O1, still strict IEEE) directly
to tmp/clip-continuous-o1.spv in session 37316 before changing generation
settings. Monitor private/commit memory, not working set alone. Generated
clip header remains stale; do not launch normal builds until regeneration.

Captured MBLACKFACE near-camera/projected-area arithmetic is now a permanent
geometry_fp64_tests fixture. Standalone rebuilt test passes exact projection
and signed-area bits, alongside 3,016,257 add/sub pairs, 1,006,158 divisions
and 1,006,158 products. This validates arithmetic, not GPU model parity.
Clip shader generation 27095 remains active.

Prepared fifteen clipping residual fixtures, including valid format-3 camera,
invalid binary64 camera rejection, and a near-clipped triangle that must emit
the exact ordered quad (112,96),(144,96),(112,64),(96,80). Shader generation
27095 remains live (DXC CPU time increasing); these added tests await rebuild.

Implemented format-3 camera consumption for backface-enabled model draws.
The clipper validates raw camera values, reconstructs screen coordinates,
and independently retains binary64 near intersections/projected vertices
through signed-area evaluation. Source traversal/cull sign is unchanged.
Renamed the enqueue option lossless_camera to reflect polygon support.
Clipping shader generation session 27095 is live; implementation is unverified
until compilation and MBLACKFACE/regression checks complete. No old sessions
are active.

Extended opt-in renderer diagnostics to capture near-clipped polygon cameras,
projected vertices and signed area (renamed capture type RenderDiagnostics).
Checker rebuild passes. MBLACKFACE isolated face 2/view 4/scale 1 CPU area is
-1.2492349924286827e-05; GPU culls it. CPU near intersection
(3535.5339059327375,3535.533905932738,0) projects to
(905208.67991878081,905192.67991878092), preserving an asymmetry lost by the
two-float camera transport. Next action: extend lossless camera transport to
near-clipped polygons and retain exact projected area; no cull tolerance change.
The separate whole-model view 1/scale 4 failure remains to be examined.

Refreshed desktop CTest 79622 passes 53/53 against the rebuilt binaries
(167.64 seconds). All compiler/build/test sessions in this batch are terminal.

Full EX axis catalog 25181 passes: 3,511 models / 63,198 images / 14,913,339
nonzero pixels; zero deferred, exact coverage/palette, zero normal/depth error.
Both axis catalogs are terminal and pass 111,744 comparisons combined. Only
desktop CTest 79622 remains running from this verification batch.

Full Original axis catalog 82374 passes: 2,697 models / 48,546 images /
8,688,525 nonzero pixels; zero models deferred, exact coverage/palette and
zero normal/depth error. EX axis 25181 and refreshed CTest 79622 still live.
This closes the Original axis catalog gate, not other model modes or VR QA.

Full desktop build 70012 passes after lossless axis transport and diagnostic
API changes. PIPE_5 repeat proof passes and its CPU-left/GPU-right capture
tmp/pipe-axis-proof/model-47775-face-0-layer-0-mode-0-view-4-scale-2.bmp was
visually inspected (matching diagonal line). Full axis scans are live:
Original 82374, EX 25181. Fresh desktop CTest started against rebuilt binaries.

PIPE_5 axis fix verified: continuous generation 66087 and build 3133 exit 0.
GPU projection suite passes all six residual-axis fixtures. PIPE_5 passes
18 images / 32,614 nonzero pixels, exact coverage/palette and zero surface
normal/depth error. Its clipped X now equals 211.5 and preserves the below-half
side for the second endpoint (19.499998092651367 as float), matching CPU
19.499999999999972 rounding. Full Original/EX axis catalogs launched next;
this targeted pass does not establish catalog completion.

Axis transport implementation now adds opt-in residual format 3: binary64
camera low words in camera XYZ, high words in screen XYZ, camera W=3. Only
GpuModel axis draws request this producer format; the reducer emits existing
screen format 2 for clipping. The reducer sums/divides raw camera values in
source order, preserves exact means through projection, validates raw doubles,
and rejects mixed input formats. Added valid/invalid raw-camera fixtures (six
axis residual fixtures total). Axis shader generation passed; continuous
shader generation session 66087 is live. No compiled/runtime pass claimed yet
for this transport change.

Added opt-in AxisRenderDiagnostics to SoftwareRenderer and the GPU checker;
the draw algorithm is unchanged and default callers pass no capture. Rebuild
and PIPE_5 trace confirm CPU axis camera (293.84271247461902,
-286.84271247461896,512), endpoint 1 camera (-271.84271247461902,
278.84271247461896,512), clipped X 211.5 and 19.499999999999972.
GPU clips X above both half boundaries. Next fix must retain the source
binary64 camera asymmetry through the axis handoff, not widen tolerance.

Windows application build 34860 completed (exit 0), including the BEAM menu
label and translations. Projection checker also passes after the new clip
header rebuild. All sessions from this precision attempt are terminal; no
pending compiler or build handle remains. No install/publish was performed.

Line precision build is now terminal: shader generation 27110 and checker
build 96525 both exit 0. Updated GPU clipping suite passes all twelve residual
fixtures plus 25,362 native polygons, 48,435,200 fractional span pixels and
24 word-near comparisons. PIPE_5 still fails coverage pixel 21619 at view 5,
scale 1: exact line clipping now produces Y endpoints exactly 0 and 192, but X
remains 211.50001525878906 and 19.500001907348633. Investigate loss of camera
bits before axis reduction/projection; do not mark PIPE_5 resolved. No shader
generation remains active from this attempt.

User-requested terminology update: the default-laser menu now reads SINGLE /
DUAL / BEAM. Updated Japanese/German/French/Spanish translations and regenerated
the menu header; weapon flags are unchanged. This is a source change awaiting
the next application build, not an installed-binary claim.

Added a twelfth clipping fixture exercising the new model-tagged exact line
path with the existing destruction half-pixel boundary case. Pending shader
generation 27110 remains live; this new fixture is not yet run. Inspected the
MBLACKFACE isolated capture: CPU draws a thin edge-on face, GPU culls it. The
BEEANIM capture shows broadly matching geometry with a fine palette boundary
difference. Neither visual inspection resolves the reported mismatch.

Latest precision continuation: session 42441 completed successfully. Its clip
and projection suites passed, but PIPE_5 axis view 5 still failed coverage at
pixel 21619. Axis screen residuals now use lossless binary64 format 2; rebuilt
projection checker passes all suites, including four updated residual-axis
fixtures. That handoff alone did not fix PIPE_5. A full-precision line clip
path is now being compiled (session 27110); it is not yet verified. Desktop
CTest (44485) and Original/EX backface catalogs (45972/3759) are running against
the preceding completed build. No release, install, or production-GPU-default
change has been made.

Those three regression sessions are now terminal: desktop CTest 53/53 passes
(148.70 seconds); Original backface catalog stops at MBLACKFACE view 1 scale 4,
coverage pixel 448; EX stops at BEEANIM face 0 view 2 scale 4 mode 0, palette
pixel 448 (CPU 9/GPU 1). MBLACKFACE isolated faces additionally stop at face 2
view 4 scale 1, pixel 19366, GPU clip count zero; diagnostic capture saved under
tmp/mblack-precision-proof. These are outstanding, not full-catalog passes.

Read-only GitHub refresh during compilation: #43/#44/#46/#47/#48 remain OPEN,
#49 CLOSED, all last updates still September 9. #44's sole comment asks about
Steam Input; #47 has no comments. No new reporter verification or external
changes to apply. No issue comments/closures were posted. Shader build 42441
still active; do not treat this issue-state refresh as completion evidence.

During 42441 regeneration, inspected axis residual counts/ownership: clipper
receives cs.point_count=2 for axis output; enqueue_axis_points takes input
pointer by value before clearing output pointer, so reusing pointer variable
does not erase its input. Last linked executable (axis transport change but
pre-new-line-shader) passes Original first64 native-axis sample, 192 images,
2,327 drawn pixels and exact surfaces. Fractional line build remains live;
this native sample does not verify the pending PIPE_5 fix.

PIPE_5 axis work: enabled exact sequential Euler for continuous axis poses and
passed camera residuals into/out of axis reduction. CPU rebuild succeeds;
PIPE_5 still fails view 5/1x pixel 21619, endpoints now 211.500015,0 and
19.5000019,191.9999847. Added source-rounded line parameter division and final
interpolation for model-tagged lines; regeneration/build session 42441 live.
No fix claimed. Check same handle before rebuilding; then PIPE_5, axis fixtures,
and previously passing TREE/TOTEM/LBLACKFACE must be refreshed.

Near-plane build 34636 completed successfully. LBLACKFACE now passes 18 images
/ 3,070,472 pixels with exact coverage, palette, depth and normals. TREE remains
passing 90 images / 500,149 pixels; new view 1/4x boundary capture generated
and inspected under tmp/tree-precision-proof. Clipping checker rebuilt and all
native/fractional catalogs, eleven residual fixtures and word-near chain pass.
No compiler from this run remains pending. Full catalogs still need refreshing
after these targeted fixes; PIPE_5 axis/line mismatch remains outstanding.

Near-plane exact-interpolation implementation is compiling in session 34636;
do not restart or build concurrently. Model-tagged line/polygon crossings use
binary64 source operation order then high/tail transport. No result yet.
Meanwhile last linked (pre-near-change) TREE run again passes 90 images and
captures tmp/tree-precision-proof. Inspected view 4/2x CPU-left/GPU-right,
matching silhouette, gaps and dither. Capture condition now also includes
destruction view 1/4x for direct boundary proof after next checker link.

Screen-intersection build 99229 completed. TREE destruction now passes all
90 images / 500,149 pixels, exact coverage/palettes, max normal 1.19209e-7,
depth .00012207. Original KICHI_0 regression sentinel also passes 90 images /
3,280,204 pixels, same normal bound, depth .000976562. Added direct three-point
TREE camera fixture proving the 93.375 boundary must round to 373 at 4x;
clipping suite passes it and all prior cases (eleven residual fixtures total).
Both 25,362 polygon sets and 48M fractional pixel comparisons remain passing.

Screen intersection precision experiment implemented: model-tagged clipping
uses integer binary64 for amount/interpolation, accurate-fragment initial
projection is source-rounded, and mode-1 final conversion preserves the side
of half-pixel boundaries even without a residual buffer. Shader regeneration
and checker build session 99229 remains live; no results claimed yet. After
completion test TREE, Original KICHI_0 destruction (prior regression sentinel),
LBLACKFACE, TOTEM_2, and clipping suite before retaining as a verified fix.

LBLACKFACE near-plane investigation: exact reprojection for area now also
handles near-clipped camera vertices. Build 96806 completed. Targeted whole
model failure changes from depth ownership at pixel 16 to coverage at pixel
12391 (view 4/1x); model remains incorrect. Near intersection construction and
screen clipping still use high/low arithmetic, so this is not full near-plane
parity. No source renderer changes were made.

TOTEM_2 proof capture generated and visually inspected at
tmp/totem-precision-proof/model-54137-face-0-layer-0-mode-0-view-4-scale-2.bmp.
CPU-left/GPU-right diagnostic colours show matching crossed faces/dither and
thin edges. Capture run passes 18 images / 471,101 pixels, exact palettes and
coverage, zero depth error. This is diagnostic comparison proof, not a headset
screenshot or proof that the default runtime uses native GPU models.
EX full-catalog backface scan 7212 still live; keep waiting its existing handle.
It subsequently terminated at LBLACKFACE with the same Original failure:
view 4/1x pixel 16, expected depth 3047 vs -2023, one visible pixel mismatch.
Both full catalog scans are now terminal. This is a shared-model issue, not an
EX-only discrepancy. No pending model-check process remains from these scans.

Full continuous-backface catalog audit started after the targeted TOTEM fix.
Original session 56386 terminated at LBLACKFACE view 4/1x: normal error zero,
depth 3047 expected vs -2023 actual at pixel 16, one visible image mismatch.
Not yet classified as new regression versus previously unaudited behavior.
EX full catalog session 7212 still runs; TREE destruction-face session 69922
also runs (output is buffered through Select-Object -Last 14). Wait those same
handles, do not restart. Updated clipping test summary to reflect ten fixtures.
TREE face session 69922 subsequently terminated: view 1/4x coverage pixel
671482; GPU intersection remains exactly 93.375,192 (source known slightly
below the 4x half-pixel boundary). This confirms remaining screen-intersection
tail loss independently of whole-model surface ownership. Only 7212 remains live.

Lossless screen handoff build 43651 completed. TOTEM_2 whole-model backface
sweep now passes all 18 images / 471,101 nonzero pixels, exact coverage and
palettes, normal max 1.19209e-7, zero depth error. Projection checker passes
393,987 native and 131,329 continuous point cases plus destruction cancellation
and axis tests. Clipping checker passes both 25,362-polygon catalogs and 48M
fractional pixel comparisons. Added three format-2 fixtures: valid payload,
valid double whose low word looks like float NaN, invalid double NaN rejection;
all pass. Printed residual fixture count still says seven but loop now runs ten.
This clears the targeted whole-model case, not all catalog/VR migration gates.
TOTEM_2 isolated-face sweep also passes 576 images / 485,671 nonzero pixels,
same normal bound and zero depth error. TREE destruction remains unchanged:
view 1/4x pixel 671482, expected depth 302 vs actual 222, zero image mismatches.

Residual interface documentation now defines format 1 high/low versus format 2
binary64 screen words. Axis reducer inspected: it consumes camera XYZ only,
checks finite camera W rather than requiring W=1, and ignores input screen;
its output stays format 1. Shader build 43651 is still live (DXC 51236 observed
at 122 CPU seconds and 21.2 GB working set). No new rendering test yet; wait
for that same build before attempting normal CMake targets.

Lossless-screen handoff implementation in progress: continuous producer keeps
exact Euler camera values through projection, emits raw binary64 X/Y words in
residual.screen with residual.camera.w=2 discriminator. Clipper validates and
decodes that format, uses raw values for backface area; format 1 still serves
legacy/axis residuals. Destruction projection checker updated for format 2.
Generator/build session 43651 remains live; no post-change geometry result yet.
Wait for that session before any normal build. Camera high/tail transport and
clip intersections remain separately lossy; this does not finish the migration.

Transport evidence: new host regression samples doubles in [1e-12,1e12]
magnitude. 91,106 / 132,707 lose source bits in high+low float reconstruction;
third residual restores every sample exactly. Existing arithmetic tests pass.
Current continuous camera and screen buffers still use two-float transport;
exact operation wrappers therefore do not ensure end-to-end double parity.
Next implement an explicit lossless buffer representation (raw uint pairs or
third residuals), updating all consumers/validation; do not reinterpret floats
as raw bits without a format discriminator (axis residuals share the interface).

Found inactive gating in the area experiment: ordinary continuous models had
projectionParams.w=0. Added mode 2 for exact-area-only reprojection; mode 1
retains existing full clipping compensation. Rebuild 97965 completed. Isolated
TOTEM_2 face 0 matrix case now passes; face scan progresses to a later view 4
failure at pixel 6316. Whole model still fails coverage pixel 5191. Original
first 64 backface models pass 1,152 images / 3,598,360 pixels with unchanged
surface error bounds. This clears one isolated fixture, not full model parity.

Diagnostic clipper now recomputes pre-clip projected area with integer binary64
from camera high/tail values when projectionParams.w==1, instead of relying
only on narrowed screen floats. Both rebuilds (33807,88830) completed. TOTEM_2
isolated face 0 still fails view 3/1x pixel 112; exact reprojection alone has
not proven the source projection convention is matched. Aggregate intermediate
test reported palette CPU 2/GPU 6 at pixel 4966. Keep this labeled experimental;
inspect source project_point operation order and actual parameter gating next.

Regeneration 80359 is now terminal, successful in 133.88 seconds; generated
source check passes. Loop experiment did not eliminate high compile cost.
Normal checker rebuild may now proceed; no pending regeneration remains.

While 80359 regenerates, previous linked executable's --backface-faces TOTEM_2
isolates face 0 failure at view 3/1x pixel 112: GPU clip count/status 0/0;
vertices 7,9,0 project X=112 with Q15 matrix coordinates. This matrix path is
not corrected by the Euler-only arithmetic experiments. Added expected/actual
palette values and face index to future palette mismatch diagnostics (not yet
rebuilt). Compiler 50380 still live, approximately 19.7 GB working set; loop
attributes have not demonstrated the hoped-for memory reduction.

Compile-cost experiment: explicit HLSL loop attributes added to binary64 limb
and quotient loops (empty macro on host). Recompiled GPU arithmetic probe still
passes 262,144 native-reference results, and freshly built standalone host tests
pass. Full continuous shader regeneration remains live in session 80359; no
compile-time improvement claimed yet. Prior generated header size was 5,251,153
bytes. Wait for this same session before normal builds; do not run concurrent
CMake while the dependency-stamped header is being regenerated.

Session 59398 completed successfully; no compiler remains pending. Third trig
residual integration again moves TOTEM_2 from coverage failure to palette
mismatch at pixel 6316 (view 4/1x), still not parity. Added actual pack_projection
coefficient reconstruction checks across sampled pitch/yaw/roll combinations,
beyond the standalone arithmetic test. Rebuilt targeted CTest passes 2/2
(geometry_fp64 and packed_projection). The shader compiler used roughly 20 GB
working set at peak during this experiment; compile cost also needs improvement
before considering the precision path production-ready.

Coefficient transport: added third sine/cosine residuals in unused Euler operand
translation fields, consumed by exact sequential rotations. Standalone rebuilt
host test confirms all 65,536 angle sine/cosine pairs reconstruct source double
bits, alongside existing arithmetic tests. Shader generation/model rebuild is
still running in session 59398; do not restart it or claim a new model result.
A premature concurrent CMake invocation rejected the not-yet-regenerated shader;
the independent host test was compiled directly to tmp/geometry-fp64-tests.exe
and passed. Wait for 59398 before rebuilding normal targets or running models.

Sequential-Euler diagnostic extension: scale, each rotation operation, and
translation now also execute via integer binary64 before high/tail transport.
Build session 33614 completed. Original 64-model backface sample still passes
1,152 images / 3,598,360 pixels, same surface bounds. TOTEM_2 remains incorrect,
now coverage pixel 4966 again. This does not establish equivalence of the packed
two-float trig inputs or high/tail intermediate transport to source doubles.
The diagnostic extension is retained for investigation, not production enablement
or a performance claim. Next isolate packed coefficient/transport rounding and
compare per-vertex source values; further blind area changes are not justified.

Exact projection experiment retained in the diagnostic native GPU producer:
continuous_portable uses integer binary64 multiply/divide/add for destruction
and sequential-Euler projection, then returns high/tail floats through tested
bit conversions. Build session 98687 completed. Original first 64 backface
models pass 1,152 images / 3,598,360 nonzero pixels, exact coverage/palettes,
max normal 5.96046e-8, depth .00012207. EX sample still stops at TOTEM_2:
failure changed from coverage pixel 4966 to surface palette pixel 6316 at
view 4/1x. TREE destruction remains unchanged (depth 302 vs 222 at 671482).
This is partial diagnostic progress, not a completed parity fix, performance
improvement, or default gameplay migration. Further transformation/clipping
precision and wider regression checks remain required.

Upstream precision work: binary64-to-float bit conversion now supports normal
rounding, gradual binary32 underflow and overflow. One million random normal
double inputs match host casts, plus explicit float subnormal/normal/tie
boundaries. This enables conversion back to existing shader buffers without
using device float64. Rendering remains unchanged; producer integration pending.

CMake now invokes the same recursive digest when a shader has includes and
tracks HLSLI changes for reconfiguration. Exact-area integration then built
successfully (session 63873 completed). Re-running the newly linked checker
still fails TOTEM_2 view 4/1x pixel 4966. Thus exact arithmetic on the current
compensated coordinates is insufficient; upstream coordinate precision needs
investigation. Removed the ineffective area change and regenerated stable
clipping, preserving the include-aware CMake fix. No model fix claimed.

Attempted exact-area integration compiles as a shader but uncovered a second
stale-source gate: CMake independently hashes main HLSL only. Integration was
reverted and the stable header regenerated; no new geometry result was obtained
(the subsequent TOTEM invocation used the old executable). CMake include hashing
must be aligned before retrying. Added exact float-bit widening helper and tested
one million random bit patterns, comparing finite values to host double casts.
Host arithmetic tests still pass after this addition.

Shader-generation stamps now include recursive local include dependencies,
preserving legacy hashes for standalone sources. Three tests verify newline
normalization, nested helper invalidation, and missing/cyclic include rejection.
Existing continuous clipping generated-source check still passes. This prevents
future geometry helper edits from silently leaving a stale generated header;
the helper is not yet connected to fractional clipping.

Portable precision groundwork: geometry_fp64.hlsli now implements binary64
addition/subtraction with 32-bit integer operations only, isolated from rendering.
The host test passes 3,016,257 operand pairs, including every normal exponent,
ties-to-even, cancellation, overflow and subnormal-result rejection. Its HLSL
compile probe builds as DXIL and SPIR-V and cross-compiles to Metal. This is
not device-execution verification or a fix for the three geometry blockers below.
Division now also passes 1,006,158 host-reference cases (random normal inputs,
every normal exponent, signed zero, divide-by-zero and range rejection).
The expanded probe again compiles to DXIL/SPIR-V and cross-compiles to Metal;
targeted CTest passes. Division uses bounded 56-step integer long division, not
a performance-validated production fast path. Device execution is still untested.
Multiplication now passes another 1,006,158 host-reference cases, including
normal exponent boundaries, signed zero and rejected out-of-range results.
It uses base-65536 limbs with bounded 32-bit intermediates and ties-to-even
rounding. The four-operation compile probe passes DXIL/SPIR-V compilation and
Metal cross-compilation. GPU execution and renderer integration remain
unfinished; these host comparisons do not clear any model parity blocker.
Device check now exists as starfox_gpu_fp64_check, taking the explicitly compiled
tests/shaders/geometry_fp64_compile.hlsl SPIR-V file. On the local SDL Vulkan
device, 65,536 operand pairs / 262,144 independent add/subtract/multiply/divide
results match native double bits exactly. This supersedes the no-device-test
qualification above for this Vulkan sample only, not DXIL/Metal/Quest hardware.
The probe does not execute geometry and does not clear the three model failures.
The helper
deliberately returns a NaN sentinel for unsupported subnormal/nonfinite inputs
or results; it is not a general-purpose complete IEEE-754 implementation.

Fixed a Metal-specific regression in the new span-mask path: generated commands
and masks both used buffer(5), because masks was missing from the generator's
explicit remap. Added masks=6 and regenerated the header. Generator now rejects
duplicate names/slots and, during generation, extra or incorrectly assigned
resources. Its --check mode audits duplicate bindings as well as source stamps.
All 20 generated portable shaders pass that audit; six independent validator
tests pass. No Metal hardware run is claimed. This took priority over starting
the proposed integer-based precision helper; subsequent groundwork is above.

Post-mask current-build blocker refresh: Original PIPE_5 --axis still differs
at view 5/1x pixel 21619 (GPU clipped endpoints 211.5,0 and 19.5,192). EX TREE
--destruction still differs in surface ownership at view 1/4x pixel 671482,
expected depth 302 vs 222, with zero visible palette mismatches. EX TOTEM_2
--backface still differs at view 4/1x pixel 4966. All three commands terminate
exit 1; no process remains pending. Unrelated wobble parity does not clear them.

Full desktop rebuild after the mask changes passes (session 91779 terminal).
Fresh CTest run completed: **52/52 pass (146.18 s)**, session 75066 terminal.
FACE_0 wobble capture sweep passes 144 images; inspected CPU-left/GPU-right
diagnostic captures at tmp/gpu-wobble-proof, mode 0 and mode 3, view 0, scale 2.
Both preserve repeated bands and sparse gaps. These are diagnostic-palette
render captures, not in-game artwork screenshots or proof of VR conversion.

New --wobble-batch tests exercise eight effect combinations, 1x/2x/4x and five
mixed model/raster layers. Original and EX first 64 models each pass 9,216
images (sessions 35595/8846 terminal), preserving black writes, metadata and
partial-command cancellation. --wobble-queued adds four changing unread
submissions before each compared frame. First-eight EX sample failed only the
nonzero-pixel fixture guard (all comparisons matched but no visible sample);
the guard is retained with a clearer error. Targeted EX SHIP_4 passes 144 images
and 506,796 nonzero pixels, exact coverage/palettes, max normal 1.19209e-7,
depth 3.05176e-5. This is queued mask reuse evidence, not a hardware VR test.
Original ANDROSS queued test also passes 144 images / 12,902,981 nonzero pixels,
with zero surface error; it exercises preserved texture prefixes alongside
forced-colour masked draws in the same reused resources.

Repeated-row mask generation now ORs contiguous runs a uint32 at a time;
edge-only modes still mark only their two authored endpoints. Original/EX
64-model effect-combination checks each pass 9,216 images after this change
(sessions 79527/44884 terminal). A second optimization retains accumulated
row bounds instead of advertising full-screen spans; its checks also pass
9,216 images per cartridge (Original 38492 and EX colour-warp 5058 terminal).
Generated SPIR-V/Metal stamp check passes. No FPS claim is made:
the checker reports CPU enqueue timing, not isolated GPU mask-generation time.

Full repeated-row GPU catalogs completed: Original **2,697 models / 48,546
images / 43,023,649 drawn pixels** and EX **3,511 / 63,198 / 88,235,793** pass
at 1x/2x/4x across six poses, alternating modes 1/3. Exact coverage/palettes,
max normal error 1.19209e-7 and depth 0.000244141; no deferred models. Sessions
41863 and 16189 terminal. Original first 64 models also pass 9,216 images with
all eight wobble/wireframe/cel combinations; session 31063 terminal.
Rebuilt VR tests pass 13/13 (13.80 s), session 79208 terminal. EX colour-warp
combinations also pass: 64 models / 9,216 images / 336,455 drawn pixels, exact
coverage/palettes, max normal 1.19209e-7 and depth 0.00012207; session 44545
terminal. Quest rebuild passes (35 s), session 99749 terminal. No install.

GPU repeated-row wobble producer connected: spans shader writes per-painter-slot
coverage masks, repeats tracer steps at the same source Y, and advances two rows
at segment boundaries. GpuClip copies the source texture prefix GPU-to-GPU into
bounded combined storage; GpuModel passes that buffer directly to rasterization.
Ordinary/textured/line/sprite paths retain their source dispatch. Packing now
sets bit 18 for solid repeated rows instead of rejecting them. Initial Original
64 and EX 64 sweeps each pass 768 images (six views, 1x/2x, modes 1/3 alternating);
ANDROSS passes 18 images with 4x added. General GPU clip suite passes afterward.
Full 1x/2x/4x catalogs are currently running: Original session 41863, EX 16189.
Do not replace/restart their executable while they are live. VR build refresh
is also live in session 98261. These are not terminal results or completion
claims; physical VR effects remain a separate path not converted by this work.

Implemented solid coverage-mask consumption in portable raster shader and CPU
replay (RasterCommand reserved1 bit 2). Aligned little-endian uint32 mask rows
live in the existing texel buffer, so no additional descriptor binding is
needed. Absolute stored X/Y bit addressing preserves separated spans and
original dither coordinates. Fixtures cover X=0/31/32/66, three rows, opaque
palette-zero writes, tags, exact normal/depth/palette ownership, zero/misaligned
stride, misaligned/overflowing offsets and oversized row count. Vulkan checks
pass plus all 168 raster cases and resident/queued submission recovery checks.
This is the consumer primitive only: GPU wobble span generation still must
allocate/write masks and connect them to the output materials. No claim that
solid wobble or the full native geometry migration is complete.

Solid wobble migration prerequisite verified: spans_portable stores one command
at polygon*height+y, but bit-0 source tracing repeatedly plots while Y remains
fixed. The recorded-raster suite now asserts mode 7 actually emits more spans
on a row than there are faces, at native/wide widths and 1x/2x/4x. All 168 replay
cases pass. Removing the gate or overwriting one row slot is not a valid port.
Next implementation needs multiple spans or a per-polygon occupancy mask
(preserving gaps, dither coordinates, write coverage and surface ownership).
The current raster shader has six read buffers, leaving room for a dedicated
mask within SDL's eight-buffer limit; this is a design option, not implemented.

GPU model packing now permits wobble bit 0 on unaffected textured polygons,
lines and sprites; affected solid polygons (including dynamic warp templates)
remain rejected. Both full packing catalogs pass. New --wobble-bypass checker
alternates modes 1/3 across six views and 1x/2x: Original ANDROSS and Original/EX
LFDIE each pass 12 GPU images with exact coverage/palette and zero surface error.
Batch failure fixtures now use non-finite coordinates rather than assuming
wobble flags reject every shape. LFDIE mixed/submitted five-layer batches each
pass 12 images, including partial-command cancellation and resource reuse.
This does not implement the solid-polygon repeated-row wobble routine.

Fractional culling experiment: replacing both CPU/GPU shoelace sums with an
origin-relative triangle fan clears TOTEM_2 view 3 but still fails aggregate
view 4 at pixel 6316. Original first 64 models/1,152 backface images pass under
that experiment, insufficient to justify changing the reference renderer.
Both implementation edits were reverted and generated shader restored. Next
work must address the remaining projection/area precision, not merely change
the CPU oracle. Live GitHub #44/#47 recheck still has no new reporter evidence.

VR cached line geometry now uses immutable shared storage like triangles.
Submission validation, exact geometry comparison, upload reuse, cache budgets
and model-update membership consume the same line view. Tests reject ambiguous
owned/shared storage and verify independent mutation cannot alter cached data.
Full VR build and 13/13 tests pass (10.51 s); native Vulkan stereo readback
passes with the line fixtures explicitly using shared storage. Quest ARM64
rebuild passes (15 s, session 94671 terminal), no install.
The 128 cache/reference mutation comparisons pass. The sampled Corneria scene
has zero cached line vertices, so its 69.16/6.85 us uncached/cached preparation
timings measure the overall cache, NOT this incremental line optimization.
No incremental FPS gain is claimed. Captures: tmp/vr-shared-lines-proof.

VR palette override precedence now bypasses the unsupported colour-warp
material stage, matching FaceMaterial's unconditional override. Both ordinary
meshes and collapsed axis lines are checked against identical non-warp packets,
including palette index zero (present optional value). Unoverridden colour warp
still fails explicitly. Full desktop VR build and **13/13 VR tests pass (9.99 s)**.

VR EX-effect dispatch now rejects only affected solid polygons, not entire
textured/line/sprite-only models carrying scanline-effect flags. Regression
fixtures cover both wireframe modes, both wobble bits, wave, cel, textured
polygons, lines, and combined flags on sprites; solid polygons remain gated
and failed builds preserve the previous output batch. Full desktop VR build
and **13/13 VR tests pass (9.27 s)**. This is source dispatch parity, not an
implementation of the remaining solid-polygon scanline effects.
Quest ARM64 APK rebuild also passes (10 s, session 40119 terminal); not installed
and not a claim of visual headset verification.

Post-backface-culling desktop rebuild passes. The direct GPU clipping check
passes 25,362 native and 25,362 fractional polygon fixtures, fractional spans
at 1x/2x/4x, seven residual boundary fixtures and 24 word near-plane comparisons.
These generic fixtures do not clear the fractional backface defect: isolated
EX TOTEM_2 face 0, view 3, scale 1 differs at pixel 112. All three projected
vertices have x=112; GPU culls the degenerate face while CPU floating-point
area accumulation retains it. Native backface catalog sweeps pass, but no
claim of fractional culling parity or default gameplay GPU geometry is made.
The full desktop CTest refresh completed: **52/52 pass (130.23 s)**, session
2819 terminal. Desktop VR rebuilt and **13/13 pass (13.95 s)**, sessions 45259
and 65078 terminal. Updated face-packing tests explicitly ensure culling stays
off for lines/sprites and pass both catalogs (2,697 Original / 3,511 EX models).
No release, push or device installation was performed.

Latest post-integration runs are terminal success: rebuilt desktop **52/52
CTest tests pass (140.36 s)**; rebuilt desktop VR **13/13 VR-labelled tests
pass (9.89 s)**. The full desktop and VR builds pass, and the Quest ARM64 build
passes separately. Sessions 25451, 44011 and 53949 are finished; earlier live
entries below are superseded. This does not close known diagnostic geometry
precision defects, missing VR effects/scene work or physical hardware QA.

Quest ARM64 refresh after native-axis changes passes (Gradle 16 s); no install
performed. Desktop CTest session 25451 remains live, first ten tests passed with
no reported failures. Desktop VR build refresh is also live in session 44011;
wait for terminal build status before running its VR-labelled tests. These
handles are specific running work, not completion claims for the overall goal.

Post-colour-warp/native-axis integration refresh: the full desktop build passes
all current build targets. A fresh parallel CTest run is active in exec session
25451; no result is claimed yet. This supersedes reliance on old test binaries
for the next regression audit. Native GPU geometry remains diagnostic-only;
the known fractional clipping/PIPE issues and broader VR delivery scope remain.

The current desktop build passes all 52 CTest tests in one fresh run (123.64 s),
including both hitlist fixtures, both ending suites, state, audio and effects.
The VR build passes all 13 VR-labelled tests (14.58 s). These validate the
existing built binaries, not physical headset behavior or completion of the
remaining GPU migration. No release or device installation was performed.

The independent libretro capture runner now accepts repeatable bounded joypad
input schedules (`--press start:600:620`) and explicitly selects the port-1
joypad. Colony stage selection and image parity remain unverified; do not
interpret an intro/title capture as Colony validation.

Reference follow-up: explicit joypad registration reaches Controls (inspected
`tmp/reference-input-menu.png`). The optional `--first-stage LEVEL2_6 --symbols`
override validates the seven-byte Original PATHSTART signature and changes only
the in-memory map pointer; it never writes the source ROM. Scripted departure
from Controls ends black at frame 2400 even without the override. Patched runs
also remain black at 4000/5000, so these are reference-setup failures, not proof
of a Colony rendering defect. Invalid input schedules are rejected in checks.

## Ray-tracing verification refresh

The rebuilt desktop executes hardware DXR 1.1 on NVIDIA RTX 5070 Ti Laptop.
Four-mode captures pass: DXR changes presentation, the removed Enhanced Shadows
override matches OFF exactly, and unavailable DXR matches native OFF exactly.
OFF/DXR images were inspected in
`tmp/ray-tracing-current-refresh/ab03d1d01e01463bb958b550ad1739c7`.
The source shadow silhouette is absent in the active DXR image; code gates
the native silhouette pass mutually exclusively with active hardware tracing.
The simulation setting initializes false. This sampled check does not establish
all-level ray-traced shadow quality or physical non-Windows capability.

The shadow verifier now explicitly checks a detached caster's ground shadow
outside its model silhouette, plus removal of that shadow without a receiver
and preservation of sky/unoccluded ground. Hardware DXR and Vulkan compute both
pass, alongside all 51 partial-workgroup fixtures and changing-scene tests.
The 1x/2x/4x DXR medians were 0.426/0.500/1.165 ms; Vulkan compute was
0.593/1.369/7.135 ms on this laptop. These are synthetic pass timings during
concurrent destruction testing, not whole-game FPS. At 4x, DXR differed from the
CPU mask at one of 1,433,600 pixels (one 20-level sample increment); Vulkan at
two pixels. The updated gameplay verifier also classifies shadowed receiver
rays on the final frame: 2,530 ground pixels and 1,815 model pixels in
`tmp/ray-tracing-receiver-proof/cb1c39b1a51c49329633086155703c3a`.
It now rejects an empty ground-shadow result rather than accepting any changed
image hash. This proves ground shadows are generated in the sampled scene;
all-level placement and artistic quality still need visual review.

## Latest VR follow-up (2026-09-10)

Implemented Mario/Luigi coplanar eye decals, EX intro full-sky atlas wrapping,
Controls emitter/background ordering, and outdoor grid height locking/shared
tilt. Added bounded immutable sky/model caches with CPU microbenchmarks and
cached/uncached equality tests. See the newest section of `VR-BUILD.md` for
evidence and limitations. All 59 numbered stage-entry preflights pass cached /
uncached comparison (600 ticks, three interpolation samples); Original and EX
intro-to-title flows pass 1,000 ticks. These are GPU-input checks, not full-level
visual validation. The Quest ARM64 build passes. No release was published; the overall goal remains
active. Remaining major work includes desktop geometry/effect migration and
default-path performance, broader immersive backgrounds beyond the completed
scenes, VR effects/menu integration, and physical cross-platform verification.

## Physical Quest verification (2026-09-10)

- Quest 3 connected and authorized; prepared Original inputs copied into its
  verified-empty app storage. Data-preserving APK install and launch pass.
- Fixed two on-device startup failures: pre-decor fullscreen access and an
  invalid empty bitmap packet transform. Packet and Vulkan regression tests pass.
- Captured the Original title in both headset eyes:
  `tmp/quest-empty-packet-proof/quest-headset.png` (inspected).
- Optimized debug native compilation (`-g -O2`) is verified in compile commands.
  A 30-sample physical window reports 71–73 FPS against 72 Hz, with some stale
  frames. Full gameplay, audio quality and input correctness remain unverified.
- Mode-2 horizontal terrain expansion passes Original/EX Vulkan captures; the
  APK is installed. Latest session reached IDLE, awaiting headset readiness;
  the new terrain change has not yet received physical visual verification.
- VR remains incomplete: vertical/world placement, HUD integration, effects,
  unsupported geometry paths and end-to-end gameplay still require work. Desktop
  native GPU geometry likewise remains diagnostic-only, not the default path.

## Save-state verification refresh (2026-09-10)

- Verified the forced-revival guard on the rebuilt executable: 100-tick
  Corneria setup exits with the explicit uninitialized-checkpoint error.
  The capture tool now supports Original as well as EX and retains its process
  handle. Original LEVEL1_1 / preroll 1000 / 1000 frames completes; frame 900
  was visually inspected and shows restored, normal-colored gameplay
  (`tmp/revival-original-checkpoint-proof/000900.bmp`). This is a sampled
  capture, not exhaustive frame-by-frame transition parity.

- Revival diagnosis: the 100-tick forced-death fixture had zero restart bank,
  pointer and background; Corneria had not executed setrestart yet. With
  1000 preroll ticks, checkpoint bank=13/pointer=2144/background=27 is valid.
  Saving at frame 900 then passes 30 restored frames and 13 non-silent PCM
  signatures with MSU enabled, in a fresh process
  (`tmp/revival-established-late`). This establishes late post-revival
  continuation, not an audio proof during the intentionally silent death phase.
  The forced-revival test hook now rejects an uninitialized checkpoint before
  changing player strategy; normal gameplay behavior is unchanged.

- Freshly rebuilt desktop suite: 50/52 initially passed; both hitlist failures
  came from a synthetic tunnel fixture missing the explicit tunnel_scene flag.
  Fixed the fixture and added a non-tunnel scanline-scroll control asserting
  normal wide tile expansion. Both Original/EX hitlist checks then pass.
  This supersedes the earlier all-green run on older executables; the other
  50 rebuilt checks passed in the same full run (141.30 seconds).

- The verifier now accepts -SaveFrame (default 20), deriving baseline/restore
  timing without changing its 30-frame and non-silent PCM requirements.
  The default Original fresh-process MSU check still passes after this change.
  A forced-revival run with -SaveFrame 300 remains silent; extending to 900
  exposes a baseline failure before any save: frame 421 / tick 80, subroutine
  $99910 escapes into unmapped ROM $448002
  (`tmp/state-msu-revival-later/baseline/runtime.log`). This may involve the
  forced-revival fixture setup and needs diagnosis; it is not a save/load
  corruption claim, nor a verified revival path.

- Original MSU revival continuation (`tmp/state-msu-revival-check`) matches
  the compared frames/PCM signatures but is rejected by the audible check:
  the selected early revival window is silent (track 38, msu-playing=0).
  Do not report this as audible revival/MSU continuation proof.

- Follow-up: the runtime explicitly gates MSU to Original at audio initialization
  and experience changes. EX MSU is therefore unsupported, not an unverified
  supported path. The continuation script now rejects EX plus -Msu before
  creating artifacts or launching the game. Original's non-silent packed-MSU
  continuation result below remains valid; EX native audio is a separate check.
- The current desktop CTest run passes all 52 registered tests (149.79 seconds),
  including isolated effects, both endings/level-clear/state/transition suites.
  This uses the existing built test executables; it does not imply native GPU
  model parity, physical hardware verification, or completion of the VR goal.

- Later MSU refresh: Original LEVEL1_1 with 100 preroll ticks and a
  fresh-process load passes 30 frame hashes and 14 matching PCM signatures
  (`tmp/state-msu-current-check`). Logs confirm non-silent track 9 with
  msu-playing=1. This exercises the existing Starfox-MSU1.PAK archive.
  The same EX check remains inconclusive: its continuation is entirely silent
  and the verifier correctly rejects it (`tmp/state-msu-current-ex-check`).
  These runs use the existing desktop executable, not the diagnostic GPU model
  renderer, and do not replace physical listening or all-track coverage.

- Fresh-process reload matches 30 rendered frames and 14 PCM blocks for both
  Original and EX (`tmp/state-current-original-recheck` and
  `tmp/state-current-ex-fresh`). Audible native audio was exercised.
- MSU-specific runs did not exercise audible MSU playback; MSU continuation
  remains unverified, not a passing claim.
- Save/load diagnostic scripts retain the process handle to avoid Windows
  PowerShell reporting a null exit code for a quickly exited child.
- The host save-slot selector's colon now draws two dots instead of the ROM
  glyph alias. The corrected EX capture was visually inspected at
  `tmp/state-current-ex-selector-colon/slot-selector.bmp`; Original simulation
  substrate tests, including the new colon pixel regression, pass.
- Android SDK license approval is recorded in the conversation; the local
  SDK/NDK is installed. Headset connection and testing now appear in the newer
  physical-verification section above.

- Source-word near clipping now executes through projection -> clipping ->
  spans -> GPU raster, with final pixels checked against SoftwareRenderer.
  All 24 depth combinations pass (-32768/-257/-2/-1 behind, paired with
  -1/0/1/2/256/32767), including fully hidden and camera-plane cases; 266,245
  nonzero pixels are covered. This is a triangle/depth fixture, not broad live
  scene proof. Full object ordering/BSP and live geometry integration remain.

- Added source-word near-plane clipping using wrapped 16-bit midpoint bisection
  and Super FX DIV2's -1 -> 0 rule, followed by source projection. The optional
  native camera buffer is consumed on-device; texture/fully-behind rejection is
  retained. Shader generation, pipeline creation and existing native/fractional
  regressions pass. The new word-near branch itself still needs execution tests
  against SoftwareRenderer before it can be treated as parity-verified.

- Continuous near-plane clipping now executes with a double-precision source
  traversal/projection oracle. The crossing fixture produces 100 compared
  clipped corners across viewport/batch-reuse cases; textured crossing and fully
  behind polygons reject correctly. Missing parameters still return status 3.
  Expanded fractional tests pass 25,362 polygons and 48,435,200 scaled-span pixel
  comparisons. This is one crossing shape plus rejection cases, not exhaustive
  real-model near-plane parity. Word-exact near clipping and live wiring remain.

- Added optional continuous near-plane polygon clipping and per-polygon
  projection parameters; descriptor bit 0 preserves source rejection of
  behind-camera texture maps. This new branch still needs its own oracle tests.
  Expanding the shader exposed unsafe optimizer changes to compensated math;
  strict IEEE compilation (-Gis) restores the existing fractional tolerances.
  Regeneration checks now require that compiler-mode marker. All existing native,
  fractional and 1x/2x/4x span tests pass; near-branch execution proof is pending.

- Fractional clipped coordinates now feed scaled solid/texture span generation
  with explicit 1x-4x scale and source-compatible rounding. XY scales before
  scan conversion; UV remains unscaled. Slanted/clipped textured and dithered
  fixtures at 1x, 2x and 4x match SoftwareRenderer across 24,217,600 pixels.
  This check replays downloaded commands to isolate emission; native resident
  raster tests also remain green. Broader poses, near-plane clipping, alternate
  EX modes, BSP ordering and live pipeline integration remain unfinished.

- Fractional screen clipping now executes against a double oracle: 12,681
  polygons pass, including invalid records and explicit near-plane routing.
  Initial FP32 interpolation amplified large-UV errors at clipped corners;
  compensated intermediate arithmetic fixes that without requiring shaderFloat64.
  Tests now enforce 0.0001 source-pixel XY and 0.001 UV error limits (maximum
  combined observed error 0.000976553). Native-chain regressions remain green.
  Fractional span generation, near-plane clipping and live integration remain.

- Added fractional screen clipping shader/runtime selection as a prerequisite
  for upscaled span generation. Vulkan/Metal generation and Vulkan pipeline
  creation pass; existing native-chain tests still pass. Fractional clipping
  output has NOT yet been executed against an oracle. Behind-camera input is
  explicitly marked for pending near-plane processing, and native span emission
  rejects fractional blocks rather than interpreting their floats as words.
  Continuous clipping tests, fractional/scaled spans and near-plane work remain.

- Added affine-textured GPU span generation, including word UV edge stepping,
  next-right-edge sampling and winding-attached endpoints. The resident raster
  accepts the texture buffer directly. A slanted quad crossing top/left/right
  native boundaries now matches SoftwareRenderer with scrolling, transparent
  texels and overlapping surface/non-surface faces across three viewports.
  The complete diagnostic still passes 31,418,112 per-face pixel comparisons
  plus final GPU colour/tag/surface checks. This is a bounded texture fixture,
  not comprehensive textured model/scale/near-plane or live integration proof.

- Strengthened the resident span/raster test beyond identical-colour coverage:
  varied per-face colours and dithering now verify painter order; distinct
  surface metadata verifies independent surface ownership. Explicit overlapping
  fixtures cover 49,260 pixels where later non-surface spans preserve earlier
  metadata. All 31,418,112 CPU-renderer pixel comparisons and final packed GPU
  colour/tag/surface comparisons pass. No live integration claim is implied.

- GPU-generated solid row spans now feed the GPU rasterizer directly in the
  same command buffer, without command upload/readback or row-bin construction.
  The diagnostic's final indexed pixels and layer tags match the source painter
  across its three viewports. Existing shared-raster regression also passes all
  168 flat/texture/line/wave/wobble/clip/sprite cases, including surfaces and
  borrowed-device lifecycle checks. This remains a diagnostic plain-solid path;
  full live geometry migration, textures/alternate emitters and BSP are pending.

- Plain solid GPU span emission now has a runtime interface and is executed
  after projection/visibility/clipping with no intermediate readback. Diagnostic
  compares emitted command coverage against the actual SoftwareRenderer for
  33-polygon batches, including invalid/hidden polygons, across native and two
  widescreen viewports with allocation reuse: 31,418,112 pixel comparisons pass.
  This verifies plain source-word fill only; texture/EX modes, scaling, compact
  command output, GPU raster consumption and live game wiring remain pending.

- Added the initial plain solid/dither GPU span-emission shader matching native
  8.8 edge tracing and extended-width accumulators. Vulkan SPIR-V and Metal
  generation pass. It emits ordered RasterCommand-compatible per-row slots
  from clipped polygon blocks, but has NOT been GPU-executed/parity-tested or
  integrated yet. Runtime dispatch, command compaction, scaled inputs, textures,
  EX alternate fill modes and downstream raster chaining remain required.

- Projection -> face visibility -> native screen clipping now has a chained
  Vulkan diagnostic: all three stages run on one command stream, with only the
  final clipped polygons downloaded for comparison. The expanded clipping test
  passes 25,362 polygons across isolated/chained modes and three viewports,
  including saturated source projection and signed-word XY/UV clipping. This
  verifies buffer handoffs, not live game integration or FPS improvement.

- Resolved all nine EX symbol-candidate decode failures rather than excluding
  them: compact previews now use the relocated ID_0_C pointer, compact LODs
  select inherited/override materials before decoding, and BSP alternate bytes
  are unsigned as in MOBJ's GETB/LOB sequence. Regression tests cover relocated
  palettes, inheritance/override and a >127 forward branch. Coverage now passes
  2,697 Original and 3,511 EX headers with zero failures; both max at eight face
  corners. This is symbol coverage, not exhaustive LOD/runtime reachability.

- Shape coverage now reports polygon corner limits across decoded faces and
  BSP batches. Original: 2,697 decoded headers, zero decode failures, maximum
  eight corners (11,560 polygon records). EX: 3,502 decoded headers, maximum
  eight corners (30,794 records), but nine candidate-header decode failures
  prevent a complete coverage claim. Eight are colour-animation failures in
  `_S` shadow definitions; HUMANA at $00e713 rejects BSP opcode 96 and shares
  its symbol name with $308000. These need definition/call-site verification,
  not a blanket decoder relaxation. LOD-only reachability also remains unaudited.

- Native-word screen polygon clipping now has a GPU shader and no-readback
  runtime interface consuming projected points, indexed UV corners and visibility
  flags. Vulkan diagnostic passed 12,681 exact polygons across three viewports,
  including signed-word XY/UV seams, exclusive borders, invalid descriptors and
  allocation reuse. Generated Metal is not hardware-verified. Input polygons are
  bounded to 32 corners; scratch overflow is explicitly reported, not truncated.
  Continuous/near-plane clipping, source asset limit audit, chained real-scene
  verification, BSP/command emission and live integration are still outstanding.

- Continuous GPU visibility now follows the fractional transform directly in
  the same command stream. Compensated FP32 determinant arithmetic preserves
  the existing 1e-12 tangent rule without requiring hardware double precision.
  Vulkan diagnostic compares 131,329 face decisions with a double oracle on
  the produced camera coordinates, including exact/near tangent triples and
  invalid points. All passed; ordinary FP32 disagreed on 37 fixture decisions.
  This proves the visibility kernel, not equivalence of all live double poses:
  real-scene comparison, clipping/BSP/command emission and integration remain.

- Added a separate continuous GPU transform/projection interface retaining
  fractional camera and screen coordinates (no native-word quantization).
  Vulkan diagnostic passed 131,329 fractional cases against a double oracle;
  maximum absolute screen-coordinate error within +/-2048 was 0.000253176
  source pixels. Invalid pose/nonfinite inputs are marked invisible, and word
  visibility explicitly rejects this path instead of reusing an older frame.
  This fixture uses dyadic coefficients, not every live pose. Continuous face
  visibility, real-scene parity and live renderer integration remain pending.

- Native source-word Q15 transform now chains into projection and visibility
  without a CPU readback. Shared pose buffers avoid per-vertex matrix copies;
  invalid pose references propagate as invisible geometry. Vulkan diagnostic
  passed 262,658 exact projected-point/face cases, including signed-word seams,
  negative-product rounding, allocation reuse and invalid indices. Generated
  Metal shader is available but not physically tested. This is still a pipeline
  building block: live renderer wiring, continuous/subpixel transforms, clipping,
  BSP ordering and GPU command emission remain unfinished. No FPS claim.

- Native GPU visibility now consumes the resident projection buffer directly
  in the same command stream, using signed-word deltas, wrapping 32-bit area
  and odd-behind correction. Invalid face indices are invisible. Windows Vulkan
  passes 131329 exact projection/visibility pairs (58648 visible), including
  allocation reuse and partial groups. Metal shader generated. No intermediate
  readback is performed by either API; the diagnostic reads final results.
  This chain is not yet selected by live SoftwareRenderer/BSP command emission.

- Native model projection migration started: GpuProjection records source-word
  MDO_PROJECT arithmetic into an existing SDL GPU command, returning a resident
  screen-coordinate buffer without submitting/downloading/waiting. Vulkan and
  Metal shaders generated. Windows hardware Vulkan passes 131329 exact points,
  covering all depth words, signed minimum, saturation ratios, wrapped vanishing
  points, partial workgroups and allocation growth/reuse. This is a tested
  pipeline building block, NOT yet connected to live native model rendering:
  transform/visibility/BSP/command emission still need their resident chain.

- Packed VR font validation expanded to all 41 source glyphs: --font and
  --font-reference independently decode source rows on GPU versus CPU-expanded
  RGBA. Original and EX final captures match exactly in both eyes; EX atlas
  visually inspected (tmp/vr-font-{original,ex}-{packed,reference}). Six invalid
  glyph payload cases reject transactionally, preserving prior uploads.
  Each unique glyph now uploads 36 bytes instead of 1024. Windows builds and
  Linux syntax checks pass; this is font-path evidence, not full VR completion.

- Deck follow-up: Steam virtual controller detection now uses Valve VID/PID
  as well as the name, takes single-player priority, and suppresses the raw
  Deck duplicate in EX player enumeration. Native fallback and custom bindings
  remain. Windows tests pass for route selection/disconnect and all standard
  button mappings, excluding Steam/Quick Access from gameplay. Physical Deck
  verification remains; see ISSUES-43-49-VERIFICATION.md for scope and sources.
- VR scaled-font preparation now uploads nine packed source/ink words instead
  of 256 expanded texels per unique glyph; bit decoding runs in the fragment
  shader. DXC SPIR-V shaders regenerated, VR diagnostic/runtime builds pass.
  EX --text stereo captures before/after are byte-identical in both eyes
  (tmp/vr-text-before-packed and tmp/vr-text-after-packed). More glyph/payload
  validation coverage remains; this does not complete VR scene migration.

- DXR mask transfer now packs four 8-bit shadow values per GPU storage word,
  preserving parallel per-pixel ray queries and padding partial rows safely.
  Aligned readback drops by 75%; CPU unpacking becomes a bulk/row memcpy.
  RTX 5070 Ti fixture medians changed from 0.383/0.813/1.934 ms to
  0.257/0.461/1.067 ms at 1x/2x/4x (separate runs, not gameplay FPS).
  Reference masks and 51 partial-workgroup/row-padding fixtures pass.
  Final DXR presentation in tmp/ray-tracing-proof/84573a9a43c5401aa535c643c80b0665
  is byte-identical to the pre-packing capture. The transfer still exists:
  SDL's pinned public GPU API exposes neither native devices nor resource
  import hooks, so direct DXR sharing requires additional backend integration.

- Latest shadow-control change: removed Enhanced Shadows from 3D navigation,
  drawing and input. Ray Tracing (still default Off) exclusively enables the
  replacement shadow scene; native silhouettes are suppressed while active.
  Old configuration entries are accepted but ignored, not converted to RT On;
  the archived legacy byte is retained for binary compatibility and a saved
  cursor on removed row 26 moves to Ray Tracing. Windows settings and full
  Original simulation tests pass. Fresh off/legacy/DXR presentation check passes
  at tmp/ray-tracing-proof/b266d222539b4483aa2b35bb1c783a62; DXR image inspected,
  with no duplicate native ship silhouette. This supersedes older notes about
  the independent Enhanced Shadows option. GPU/VR migration remains unfinished.

- Original regional title logos: source Japanese and Starwing assets located;
  Europe/Germany share the source Starwing logo. Japanese/German language
  selection is implemented, with French/Spanish retaining the US logo. All five
  selections retain their tiles/maps/palettes across 40 further title ticks.
  Isolated BG3 captures in tmp/title-regional-proof were visually inspected for
  Japanese, German and US; EX exclusion passes. English (Europe) is now the
  sixth language choice (ID 5), preserving existing IDs and English text/fonts.
  It selects Starwing and the coloured controller palette; settings/state
  validation accepts the new ID. Full Original simulation/controller tests,
  settings round-trip tests and regional title re-entry tests pass. The full
  GPU title capture tmp/title-full-scene/language-5.bmp was visually inspected.
  Save-state tests now round-trip all six IDs and commit restored state before
  title/intro/re-entry checks. Original/EX archive round trips pass; Original
  post-restore title re-entry and menu left/right/A cycling tests pass too.
  Fresh full GPU title captures after 120 presentations at 60 FPS are now
  available in tmp/title-full-scene (language-1 Japanese, language-2 German,
  language-0 US). All three were visually inspected with full character/model
  art, stars and title text present. These close the isolated-layer-only
  screenshot gap, but not title-exit/re-entry or English/Europe UI selection.
  Japanese/German title-exit/re-entry is now covered by source-driven tests:
  wait for the native attract intro, skip it via Start, return to title and
  advance 40 further ticks; exact tiles/maps/palettes survive. Original test
  and EX-exclusion test pass. English/Europe UI selection remains open.

- Android fullscreen: native windows now request fullscreen at creation and
  fullscreen hotkeys cannot switch Android back to windowed mode. The Activity
  hides system bars on create/resume/focus using API 30+ insets control or
  legacy sticky immersive flags, with cutout layout enabled. APK compilation
  and device proof (gesture/three-button navigation, rotation and resume) remain
  pending; no Android SDK/JDK or connected device was found in this environment.

- GPU migration: resident main-scene raster/composition/effects/presentation
  implemented, enabled by default on supported Vulkan/Metal backends, and
  captured in Original/EX at 1x/2x/4x without GPU test overrides. Broader transitions
  and console backends remain. Portable shadow masks now feed
  effects directly on-device; DXR's separate-device mask transfer remains.
  Bomb colour disks now stay GPU-resident; Original/EX 60 FPS captures match
  CPU at 1x/2x/4x, and 864 arithmetic/clipping cases pass on Windows/Linux GPU.
  Game Over background subtraction is also resident; 864 GPU cases and
  36-frame Original/EX CPU comparisons pass. Other overlay fallbacks remain.
  Fixed-colour math is resident: 1,728 cases pass on Windows/Linux, and all
  120 sampled EX death/revival frames match the CPU path. Non-horizontal
  window masks are now resident too: 1,728 Windows/Linux cases pass and a
  fresh 120-capture revival comparison passes without transition readback.
  Live FPS overlays are now GPU-resident: 648 Windows/Linux cases and
  Original/EX 1x/2x/4x captures pass without overlay readback.
  Save-slot/exit panels are resident too: 1,296 combined overlay cases pass
  on Windows/Linux; Original slot and EX exit captures match at all three scales.
  Setup/menu ink and dimmed panel composition are now resident on SDL GPU;
  648 Windows resident/uploaded pixel cases and nine full-size aspect/scale
  cases pass on Windows and Linux Vulkan. Touch composition is resident too:
  108 clipped cases and nine full-size setup/touch ordering cases pass on both.
  New runtime screenshots and physical Metal/mobile checks remain.
  Planet isolation/level fades now have an on-device stage: 1,728 resident/uploaded
  cases pass on Windows hardware Vulkan and Linux lavapipe. Planet/text overlay
  composition now stays on-device with all six filters; Windows/Linux pass 1,944
  resident/uploaded cases and nine full-size combined/deferred cases. Fresh game
  captures and physical Metal checks remain; this is not full migration completion.
  Separated model smoothing now consumes resident surfaces without raster readback;
  216 direct base/model/glow target comparisons pass on Windows/Linux, with an
  independent CPU model/edge reference in 108 cases. Final SDL target/window
  captures remain; native projection/BSP is still CPU work.
  Optional GPU effect textures now allocate on demand: the plain 400x224/4x
  fixture drops from 30,464,032 to 11,468,832 texel payload bytes. Windows/Linux
  allocation lifecycle and pixel regressions pass; this is not an FPS claim.
  Empty staging placeholders are also removed: 108 Windows/Linux resident
  effects cases perform no staging upload; real overlay/shadow payloads remain.
  Removed unused duplicate CPU transforms/projections in the upscaled path;
  12 before/after runtime captures match and geometry/shadow regressions pass.
  CPU projection/BSP migration itself remains unfinished.
- VR: Vulkan binding, eye swapchains/depth targets, fenced stereo submission,
  native model transforms, face/BSP membership, lines, textures and sprite-face
  billboards are implemented in the diagnostic path. Both-eye physical Intel
  offscreen checks pass; injected lifecycle tests cover failure cleanup.
  `--render-model ROM SYMBOLS SHAPE` now connects decoded cartridge batches to
  tracked OpenXR eye rendering. No active headset runtime locally, so actual
  headset submission remains unverified. Basic controller action creation,
  binding, focus-safe polling, roll and a native input latch are implemented.
  A paced frame driver matches Original/EX simulation state at 90 Hz headset
  cadence in unlocked mode, without double-stepping eyes. Owned live scene
  snapshots now preserve draw order, object generations, camera and palettes
  without changing emulated bus state. Native draw packets now share source
  lighting with software rendering and pass real Vulkan cartridge captures.
  Multi-object GPU submission is implemented and used by the OpenXR model
  diagnostic; independent transforms/textures, depth and line-only packets
  pass real Vulkan checks. Live source-tick model assembly now produces Original
  and EX packets using native LOD/material tables. Interpolated pose assembly
  reuses lifetime/reticle/overlay rules and passes paced simulation checks;
  full scene passes and dynamic GPU/runtime integration remain outstanding.
  Frozen live LEVEL1_1 model layers now have real Vulkan stereo captures for
  Original and EX, using source palettes (see VR-BUILD.md).
  A paced live-game OpenXR model diagnostic now connects input/interpolation
  and dynamic submission, but physical runtime testing is blocked by the
  missing active OpenXR runtime. Exact per-object geometry/pipeline reuse now
  survives draw-list reorder/insertion/removal using object keys, with real
  Vulkan checks. Native PCM device output uses the desktop bounded queue;
  dummy-device lifecycle tests pass, but audible/headset routing is unverified.
  Optional MSU packs now feed the shared desktop/VR stem mixer; Windows/Linux
  fixture PCM parity tests pass. Actual soundtrack/headset playback is unverified.
  Source no-op faces no longer reject otherwise valid VR models; software pixel
  parity and Original/EX mesh fixtures pass. Explosion/EX effects remain pending.
  Complete world/HUD scene assembly, controller remapping, physical headset
  checks, special model effects and raster parity remain. Not yet playable VR.
- Ray tracing: separate default-Off option uses hardware DXR 1.1 shadow rays;
  independent of Enhanced Shadows. Not a path-traced GI/reflection renderer.
- Issues 43, 44, 46, 47, 48, 49: see ISSUES-43-49-VERIFICATION.md. Issues 43,
  46, 47, 48 and 49 are fixed locally; 44 remains. Issue 47 has matching
  Windows results-screen captures; Android-device verification remains.
- Save/load optimization: RAM serialization now uses format-identical bulk copies; Original/EX
  continuation passes on Windows/Linux. Separate archive microbenchmarks are
  recorded in SAVE-STATES-STATUS.md, not claimed as gameplay FPS improvements.
- Visual proof: real runtime captures wherever possible; test/manifest evidence
  for nonvisual changes. Do not substitute generated images for proof.
- ScaleFX 2D filter: reference located at libretro/slang-shaders,
  edge-smoothing/scalefx. All five reference passes are ported to compute and
  compile to SPIR-V/Metal source; notices retained. GPU-resident dispatch,
  C++ fallback, settings/menu integration and real EX captures implemented.
  CPU/GPU results agree across 60 cases and masked composition at 1x–4x.
  Independent GLSL reference and physical Metal/console verification remain.
  See SCALEFX-STATUS.md.
- Post-scramble reveal: smoothly interpolated straight opening at 60+ FPS.
  Horizontal band-edge interpolation implemented; 60 FPS 4:3 and 240 FPS
  widescreen source-program captures saved. See PRESENTATION-PARITY.md.
- Comms: source-controlled teammate health-bar visibility; avatar window
  source meter gate and text displacement implemented and captured with/without
  the meter. Corrected mislabeled 8:7 presentation to 4:3 for the native canvas,
  with pointer-coordinate tests and software-renderer capture. Widescreen
  comms portraits now receive matching 7:6 horizontal pixel correction without
  moving their text/meter or custom HUD anchors. Original/EX widescreen runtime
  captures and 1x/2x/4x/10x pixel tests cover the correction. Native-width GPU
  presentation-readback evidence remains open.
- OPTIONS > CHEATS: implemented God Mode, Level Select, Default Laser
  (single/dual/fully upgraded), Infinite Bombs and Infinite Boost. Settings
  persist; selectors use press edges and Back cannot leak a held confirmation.
  Level Select enumerates available normal route/stage symbols in the active
  cartridge and launches through its native initializer. Starting lasers are
  upgrades, not permanent locks; resource cheats do not enable God Mode.
  Windows Original/EX gameplay/menu regressions and desktop/UWP settings tests
  pass, as do Linux Original/EX gameplay/menu and settings tests.
  Menu capture: tmp/cheats-menu-proof/cheats.bmp.
- Save states: Ctrl-F1 save, Ctrl-F2 load, Ctrl-F3 opens in-game slot-selection
  window. In progress: explicit little-endian archive and checksummed,
  schema/cartridge-bound container; transactional object-pool, particle and
  dust component restores. Windows/Linux component tests cover corrupt and
  truncated data, allocation identity/order and deterministic continuation.
  CPU/PPU and map serializers now preserve suspended tasks, latches, calls and
  loops, with Original/EX map continuation tests. MSU restores fractional
  cursors and track identity with sample-exact Windows/Linux tests. SPC/DSP,
  filter and carry-buffer components now restore with audible continuation
  tests. Whole-game host flow and transactional commit now pass Original/EX
  gameplay and frontend/audio continuation tests on Windows/Linux. Runtime
  input/clock/mixer integration, atomic slot files, hotkeys and slot UI are
  implemented. Original/EX same-process and fresh-process Windows loads pass
  with real selector captures. Broader runtime cases and console verification
  remain; see SAVE-STATES-STATUS.md.
- Audit every level's 16:9+ background side expansion: missing coverage,
  incorrect colours and repetition of graphics that should remain centred.
  Include the specific Game Over/Titania issue cases above. Original entry
  sweep captured 19 stages at three widths; fixed Macbeth's incorrectly
  blacked-out outer landscape. EX entry sweep captured 40 stages at three
  widths; fixed LEVEL3_4's duplicated distant planet. Special-route entry
  snapshots are captured too. EX LEVEL6_2 water stripes/edge clamping are fixed.
  EX LEVEL6_4's isolated background is clean; its wedge is in a later layer.
  EX LEVEL5_4's duplicated planets are fixed with colour-aware sky masks.
  Original 6,000-tick sweep captured 19 stages at three widths (57 images),
  inspecting all 19 ultrawide samples; LEVEL2_6 remains malformed natively.
  Restored source BG3 camera-scroll capture/publication; Original/EX full
  simulation-data regressions pass on Windows/Linux. This does not resolve
  LEVEL2_6's asymmetric tilemap.
  EX LEVEL4_4 findings, LEVEL2_6,
  later-stage backgrounds and natural transitions remain. See BACKGROUND-AUDIT.md.
- Upgrade pickup wireframe overlay: interpolate in sync with the ship,
  including high-FPS presentation and source-frame boundaries. Anchored the
  source FLASHPLAYER effect to player history; 240 FPS motion/blink tests and
  a turning-ship capture cover the fix. See PRESENTATION-PARITY.md.

Existing release instructions are not authorization to publish these unfinished
changes. Preserve the user's saved configuration and game data during QA.

- VR widened-background verification: explicit logical horizontal bounds retain
  source UV coordinates instead of stretching the native image. Empty, maximum
  and invalid bounds pass packet tests. The real EX LEVEL1_2 400-pixel BG2
  diagnostic passes 478 CPU-decoder sample comparisons in both Vulkan eyes
  (140 coloured samples), with captures in tmp/vr-space-wide-ex. This verifies
  sampled tile decoding, not whole-scene parity or headset coverage; runtime
  expansion policy remains pending. Original also passes the same 478 both-eye
  comparisons (140 coloured samples), captured in tmp/vr-space-wide-original.
- Fixed the live OpenXR background/HUD reuse path to update model matrices even
  when geometry is unchanged, so source vanishing-point changes do not leave
  reused layers at stale positions. Windows runtime diagnostic builds; physical
  headset validation remains unavailable.
  The transform-only Vulkan regression now derives its displacement from two
  source-layer vanishing points: both eyes move approximately 15.21 pixels as
  expected without geometry replacement (tmp/vr-source-plane-reuse). This
  validates GPU matrix updates, not the complete live headset transition.
- VR ground dots now use the same interpolation fraction as models/stars,
  interpolating camera/view before source word quantization and lattice wrapping.
  Scene/mode/discontinuity changes snap to current state. Original and EX pass
  17 intermediate-position source-lattice comparisons and scene-transition reset
  checks, plus existing paced simulation tests; runtime diagnostic builds.
  Connected EX grid lines remain unsupported, and physical visual proof remains
  pending. This does not move remaining CPU grid transforms onto the GPU.
- Added a separate GPU grid-transform path: 12 source words describe the lattice
  origin and Q15 view matrix; vertex shaders perform per-product shifts, 16-bit
  wrapping, depth clipping and secondary-dot selection. The `--grid-gpu`
  diagnostic matches `--grid` byte-for-byte in both stereo BMPs for the identity
  view fixture (tmp/vr-grid-transform-gpu and tmp/vr-grid-transform-cpu).
  Shader generation/check and Windows diagnostic builds pass. Varied rotations,
  wrapped camera positions and live interpolated integration remain before this
  replaces the CPU reference path; no performance improvement is claimed yet.
- Follow-up grid migration: rolled-view and wrapped-camera/yaw fixtures now
  match the CPU-reference stereo BMPs byte-for-byte too (tmp/vr-grid-rotated-*
  and tmp/vr-grid-wrapped-*). Live OpenXR ground dots now select the GPU path
  with interpolated source payloads. Original/EX 17-phase payload tests and
  paced simulation regressions pass; Windows runtime diagnostic builds.
  CPU reference remains available; connected grid lines, physical headset
  validation and performance measurements remain outstanding.
- GPU scene uploads now reuse vertex and texture/payload buffers independently,
  with immutable shared ownership preserving transactional replacement. Camera
  payload changes no longer force unchanged grid vertices back onto the GPU;
  vertex changes likewise retain unchanged textures. Hardware Vulkan tests
  verify upload counts 0/1 (payload-only), 1/0 (vertex-only), and 1/1 (restore),
  followed by passing stereo pixel checks (tmp/vr-independent-buffer-reuse).
  Windows runtime/scene diagnostics build. This is upload reduction, not an
  established gameplay FPS improvement.
- Combined EX LEVEL1_1 verification now includes the GPU grid path: both
  full-scene stereo captures match the prior CPU-grid composition byte-for-byte
  (tmp/vr-composite-grid-cpu versus tmp/vr-composite-grid-gpu). The GPU left-eye
  image was visually inspected: the central background plane still leaves a
  pink outer backdrop and near-camera shadows exposed. This is migration parity
  evidence, not acceptable final VR coverage or a playable-headset claim.
- Grid payload validation now checks consecutive references to the same 12-word
  source block once per packet, rather than repeating its word scan for all
  2,700 vertices. Per-vertex bounds/flags remain checked; fractional/nonfinite
  lattice indices are rejected. Hardware Vulkan tests cover truncated blocks,
  out-of-range words, fractional indices and conflicting flags, preserving the
  previous scene on rejection; stereo regression checks pass.
- Grid preparation benchmark added (128 camera updates, Release diagnostic).
  Initial run: CPU packets 89.44 us/update, GPU packets 133.82 us/update. A
  copied-template cache experiment did not establish a gain (135.79/150.16 us
  CPU/GPU in its run) and was removed. These noisy microbenchmarks expose a
  remaining CPU packet-construction/copy cost; GPU upload reduction alone is
  not evidence of faster frames. Shared immutable geometry needs consideration
  before claiming this migration as a performance win.
- Implemented shared immutable vertex ownership for GPU grid packets. Camera
  updates now retain the same vertex allocation; material changes create a new
  allocation without altering retained frames. GPU uploads and transform-only
  runtime paths consume the shared vertex view. Release packet preparation was
  1.35 us/update in the new 128-update run (CPU reference 112.43 us); this is
  a microbenchmark, not gameplay FPS. Both stereo BMPs still match the CPU
  reference exactly (tmp/vr-grid-shared). Ownership/interpolation regressions
  cover retained material data and shared camera-update geometry.
- Shared-geometry follow-up: packet equality rejects ambiguous dual storage;
  owned/shared representations with identical vertices reuse GPU geometry.
  Hardware Vulkan tests verify zero replacement uploads on representation-only
  changes and preserve the old scene on ambiguous input. Packet unit checks and
  stereo regressions pass (tmp/vr-shared-ownership); mesh diagnostic also builds.
- Added diagnostic GPU starfield transforms (`--dust-gpu`): shader-side wrapped
  camera deltas, view transform, depth clamp/culling, secondary-dot selection,
  and source depth/phase colour lookup. Source colour tables retain CPU material
  conversion in the small payload. Initial five-point fixture matches CPU
  stereo BMPs byte-for-byte (tmp/vr-dust-transform-gpu and -cpu); shader checks
  and Windows build pass. Rotated/fractional-camera and diverse-colour coverage,
  payload rejection tests, performance work and live integration remain.
- GPU starfield follow-up: eight malformed payload cases now verify transactional
  rejection (truncation, nonfinite camera/colour, matrix range, phase, secondary
  dot, conflicting flags, fractional source point). A 64-star rolled-view fixture
  with varied source palette/depths matches CPU stereo BMPs byte-for-byte
  (tmp/vr-dust-colours-gpu and -cpu). Fractional/wrapped camera coverage and live
  integration remain; this does not establish all colour-table entries on GPU.
- Starfield GPU integration: fractional camera motion and signed-coordinate
  wrapping fixtures match CPU stereo captures exactly (tmp/vr-dust-motion-*
  Original, tmp/vr-dust-wrap-* EX). Live OpenXR now selects interpolated GPU
  starfield transforms. The full EX LEVEL1_2 scene also matches its CPU-dust
  baseline byte-for-byte in both eyes (tmp/vr-space-dust-*). Runtime diagnostic
  builds; actual headset motion and performance remain unverified.
- GPU star vertices now share immutable storage until active source points/count
  change; camera/palette updates do not rebuild them. Recycling creates a new
  allocation without modifying retained packets. Original/EX ownership tests
  pass, and stereo BMPs remain exact CPU matches (tmp/vr-dust-shared). In the
  64-star, 128-camera-update Release microbenchmark, preparation measured
  3.08 us GPU-path versus 14.77 us CPU-path. This is not a whole-frame FPS claim.
- Starfield cache transition tests now cover counts 0/1/120/511/back to 4,
  checking all active phase selectors, palette-only payload updates with vertex
  reuse, and rejection of 512 points. Original/EX paced-state regressions pass.
  These are packet/state tests, not physical-headset 511-star visual proof.
- Added a hardware-rendered 511-star EX fixture with varied positions, depths
  and colours under a rolled view. GPU/CPU stereo BMPs match byte-for-byte
  (tmp/vr-dust-511-gpu and -cpu; 210/214 lit pixels). Its 128-camera-update
  preparation microbenchmark measured 3.37 us GPU-path versus 404.24 us CPU-path
  in this run; it excludes GPU execution and is not a frame-rate measurement.
- Original combined LEVEL1_2 scene now also matches CPU-starfield composition
  byte-for-byte in both eyes (tmp/vr-original-space-gpu and -cpu). Added the
  retained `--live-space-cpu-dust` diagnostic reference mode for future
  comparisons without source edits. Updated runtime and scene diagnostics build.
- Added tools/check_vr_starfield_parity.ps1 to repeat basic, colour, fractional
  motion, wrapped-coordinate, 511-star and combined-scene comparisons for both
  cartridges. Each run creates a fresh evidence directory, rejects nonzero
  diagnostic exits/missing images/hash differences, retains logs and stereo
  BMPs, and writes a result manifest with the diagnostic executable hash. It
  does not build the desktop executable or claim headset/full-game parity.
- Connected-grid investigation: the host deliberately skips native MSHOWGRID
  dispatch in wdc65816.cpp; therefore reading M_PREVX/M_PREVY from emulated RAM
  would not recover the desktop renderer's live endpoint history. That history
  lives in DustRenderer (previous endpoint plus per-source-frame start point).
  MSHOWGRID2-style lines also use asymmetric leftward stepping and special row
  wraps, not ordinary static lattice edges. VR still explicitly rejects this
  mode. A source-frame-owned endpoint history is required before shader line
  emission; do not substitute stale native words or a generic wire grid.
- Extracted GridLineHistory as a reusable source-frame endpoint component and
  switched DustRenderer to it without changing line emission. Tests cover
  repeated presentations/eyes, endpoint carry, stale completion and explicit
  reset. The standalone shape-decoder suite passes, including connected-grid
  repeated-frame pixel equality. VR endpoint capture and GPU line emission
  remain pending; this extraction alone does not enable connected grids in VR.
- Extracted project_source_grid: bounded source-order visible points without
  rasterization/history mutation. Connected-grid desktop emission now uses it.
  Nine width/wrapped-camera cases match the independent dot-grid renderer pixel
  for pixel, and existing connected-line/repeated-frame regressions pass.
  This prepares canonical endpoint capture; projection is still CPU work and
  does not complete VR connected-line emission.
- VR snapshots now own canonical connected-grid starting endpoints for the
  224x192 source viewport. History advances transactionally at capture, not per
  eye/presentation. EX tests require a visible ground-grid fixture and verify
  endpoint carry, unchanged complete VM state and retained old snapshots.
  Runtime diagnostic builds; GPU connected-line emission still remains pending.
- Derived a closed-form connected-grid pixel sampler for eventual shader use.
  2,145 direction/slope combinations match the existing incremental source
  stepping rule at every emitted pixel, including negative DX, steep lines,
  zero length and exact error boundaries. Uses widened arithmetic for endpoint
  extremes. It is not yet wired into GPU line emission or presented as its fix.
- Ported the connected-line sampling rule to a dedicated GPU fragment path.
  64 sampled on/off-line checks match the tested CPU sampler in both eyes
  (tmp/vr-grid-line-reference), including negative/zero DX and steep slopes.
  Endpoint payloads are bounded for shader arithmetic. Shader generation/check
  and Windows diagnostics pass. Full line geometry, source-plane placement and
  live connected-grid integration remain; sampled checks are not full-line proof.
- Added diagnostic connected-grid packet assembly: clipped line bounding quads,
  source endpoint payloads, primary/near markers and guarded source-plane
  placement. CPU evaluation of its quads matches the complete 224x192 native
  connected-grid raster in the EX fixture. This is packet-geometry evidence;
  actual full-grid GPU capture, interpolation and live integration remain.
- Full connected-grid GPU captures now match native raster reference quads
  byte-for-byte in both eyes for the straight-view EX fixture
  (tmp/vr-connected-grid-gpu and -reference). Added retained rotated-camera
  diagnostic modes. These expose a remaining raster discrepancy: left eye
  misses two pixels; right eye misses two and adds one
  (tmp/vr-connected-rotated-gpu and -reference). Do not enable the live path
  or claim full connected-grid parity until this discrepancy is resolved.
  Tests are offscreen source-plane comparisons, not headset verification.
- The parity script now accepts -Suite ConnectedGrid. A fresh EX run confirms
  straight-view equality and fails on the rotated case, retaining both captures
  and diagnostic logs in tmp/vr-connected-regression/342cab888af247ee9421b2d053f5311f.
  Differing pixels are (105,117)/(105,142) in the left eye and
  (150,162)/(150,163)/(150,167) in the right eye. Their shared screen columns
  suggest raster boundary/UV sampling; this is a hypothesis, not a proven cause.
- User resolved the desktop quarantine; the current Windows starfox_pc target
  rebuilds successfully. Resumed default resident-pipeline verification with
  the save-slot overlay: Original at 1x/2x/4x produces exact CPU/GPU capture
  hashes, requires the GPU panel-overlay log and rejects CPU overlay readback.
  Evidence: tmp/gpu-native-restored-slot-recheck; visually inspected the 2x GPU
  capture. Fixed the test runner's Windows PowerShell process-handle lifetime
  so short-lived children retain a real exit code rather than a null result.
  This validates panel rendering, not complete save/load or all-game parity.
- Restored desktop EX enhanced-shadow check passes exact CPU/GPU capture
  parity at 1x/2x/4x over 64 presentations after 1000 source ticks; the runner
  requires resident compute-shadow backend logs. Evidence is retained at
  tmp/gpu-native-restored-ex-shadows. This is a bounded scene test, not all
  shadow workloads or full frame-rate certification.
- A native-width comms attempt also matches captures at three scales
  (tmp/gpu-native-restored-comms), but visual inspection shows an empty portrait
  frame, so it does NOT close the portrait presentation proof requirement.
  Need source-message timing that actually displays the avatar before claiming
  the pending native-width GPU portrait check passes.
- Correcting comms fixture timing to 60 FPS/60 presentations with startup
  preroll skipped yields a visible Falco portrait. Original default resident
  GPU and CPU raster captures match at 1x/2x/4x; the 2x capture was visually
  inspected (tmp/gpu-native-comms-timed). Added -Message to the native check
  runner to retain the required timing setup. These BMPs are source-raster
  readbacks, not the final aspect-corrected GPU presentation target; that
  distinct display-aspect proof remains pending.
- The subsequent final-target test closes the Original 2x native-width GPU
  display-aspect evidence gap: fresh GPU/software 597x448 presentation BMPs
  are byte-identical, with a visible Falco portrait and resident-pipeline log.
  Both images were inspected. See PRESENTATION-PARITY.md for images and hash;
  this does not imply all-platform or all-language portrait verification.
- Added a live separated-model fixture and required backend/readback checks.
  It exposed a hard-coded false passed to the presenter's smooth-polys option;
  this now forwards game.smooth_polys(), consistent with surface allocation.
  Defaults remain unchanged. The Windows build and Original 1x/2x/4x runtime
  captures pass with the GPU separated-model log and no CPU transition readback
  (tmp/gpu-separated-runtime-fixed). Final combined SDL target comparison and
  EX coverage are still required; raw capture equality alone is not that proof.
- Original and EX separated-model final presentation captures now match their
  CPU references at 1x/2x/4x, with resident model-layer logs and no transition
  readback. Evidence: tmp/original-separated-final-visible and
  tmp/ex-separated-final-visible. The EX 2x final image was visually inspected.
  Initial captures hit startup black frames and are explicitly not proof;
  -PresentationCapture now skips startup presentation, saves distinct per-mode
  final targets and rejects uniform captures before comparing hashes.
- Added independent --connected-grid-texture[-rotated] diagnostic modes:
  native DustRenderer raster sampled as one nearest framebuffer texture,
  retaining the earlier per-pixel-quad reference unchanged. Rotated EX evidence
  at tmp/vr-connected-texture-rotated differs from GPU line emission by two
  pixels in each eye. It matches the old reference in the left eye but differs
  by five pixels in the right. Thus per-pixel tessellation introduces its own
  sampling differences, but does not explain away the GPU line discrepancy.
  Both references remain available; live connected grids are still pending.
- Isolated the rotated-grid discrepancy with --connected-grid-common-rotated:
  keeping the same endpoints, line sampler and markers but expanding every
  line rectangle to the common source canvas makes both eye images match the
  independent framebuffer-texture reference exactly. Evidence:
  tmp/vr-connected-common-rotated versus tmp/vr-connected-texture-rotated.
  Thus the tested mismatch comes from per-rectangle interpolation, not the
  integer stepping rule. This excessive-overdraw mode remains diagnostic only;
  production needs consistent source sampling without full-canvas work per line.
- Implemented a binned connected-grid packet: one common-coordinate quad,
  per-row candidate lists, GPU integer line sampling and marker tests. It
  avoids separate full-canvas line draws and fixes the rotated reference
  discrepancy in both eyes (tmp/vr-connected-binned-rotated). Upload validation
  bounds every row, primitive and endpoint. The original line/pixel/texture
  diagnostic modes remain available. Live integration, broader endpoint-history
  coverage and performance measurement remain; initial bin construction still
  reuses CPU-projected diagnostic rectangles and does not finish GPU projection.
- Removed that temporary rectangle construction from the binned builder. It now
  emits primitive records directly from projected points, clipping monotone
  line row ranges using only two endpoint samples; per-pixel work stays GPU.
  All eight Original/EX stereo comparisons still pass at
  tmp/vr-connected-direct-bins/20ef8c93279d4592b2f724291d39279a.
  Added a complete 224x192 row-bin evaluation versus the independent native
  raster; EX paced-state tests pass. CPU projection and live integration remain.
- Expanded the complete connected-grid raster oracle to 16 changing camera
  frames with alternating rotated/identity views and carried endpoints. Each
  frame also repeats native drawing and packet assembly to detect accidental
  per-eye history advancement. Original and EX paced-state executables pass
  all 16 full-raster comparisons plus their simulation/audio state checks.
  This covers completed-source-frame history, not fractional-camera rendering
  or physical headset ground placement.
- Added fractional-camera connected-grid assembly with the same finite-alpha,
  clamping and transition/discontinuity snapping rules as dot grids. Every
  presentation uses the current source frame's owned starting endpoint.
  EX tests compare 17 camera phases across each of 16 history frames against
  direct source-word projection, deliberately poisoning the previous endpoint;
  phase equality and transition snapping pass. GPU projection, live world
  placement and physical headset proof remain incomplete.
- Real Vulkan validation tests now reject 11 malformed connected-grid row
  payloads (header/list/record bounds, conflicting flags, excessive counts,
  invalid primitive types and endpoint limits). Every rejected update preserves
  both original uploads, verified by zero-upload reuse on resubmission. The
  valid row fixture and subsequent stereo capture still pass at
  tmp/vr-connected-row-validation. This is defensive upload validation,
  not live gameplay or headset verification.
- Connected-grid selection is now wired into the live OpenXR diagnostic and
  combined offscreen scene path, retaining pass key 0x30000 and source-owned
  interpolation history. Both diagnostic targets build. A new forced-source
  --live-connected-grid EX fixture selects flag-512 geometry and renders the
  combined scene successfully (tmp/vr-live-connected-grid); left-eye capture
  inspected. This exposes the known bounded background plane and extraneous
  visible shadow placement. Connected lines still occupy a source plane rather
  than complete 3D ground; no playable/headset-ready claim is made.
- Shadow isolation: the native host and VR both clear rotation components
  1/4/7 and place non-true-colour shadows at SHADOWHEIGHT before view transform.
  Added --live-connected-grid-without-shadows as a diagnostic-only snapshot
  ablation. Its capture (tmp/vr-connected-no-shadow) removes the oversized
  lower silhouette while retaining the same background/model scene, confirming
  that silhouette belongs to the shadow pass. This does not prove a transform
  defect; scene ground/background coverage and clipping still need resolution.
  Normal runtime shadows remain enabled and unchanged.
- ScaleFX independent reference proof added: export RGBA and intermediate float
  fixtures from starfox_scalefx_check; execute original five-pass GLSL through
  tools/check_scalefx_reference.py on Mesa OpenGL/EGL. All 24 distinct fixtures
  match final pixels exactly; all 60 Vulkan cases also pass. Initial mismatch
  was a reference-runner corner-toggle parameter error, now corrected, not a
  production filter bug. Physical Metal/console verification remains separate.
- Ray-tracing visibility investigation: added starfox_dxr_check and a
  nonempty-shadow assertion. Real DXR 1.1 on the RTX 5070 Ti passes at 1x/2x/4x
  (8023/31745/127431 shadowed pixels); CPU-reference differences are 0/0/1 pixels.
  Median dispatch/readback times: 0.383/0.813/1.934 ms (fixture, not game FPS).
  tools/check_ray_tracing.ps1 captures off/enhanced/DXR final presentations in
  fresh directories, requiring a hardware-backend log and changed off/DXR image.
  Proof: tmp/ray-tracing-proof/8d93c65ba3164503bfa2376623388aed.
  Off and DXR images inspected: native silhouette replaced, model shading changed.
  Enhanced and DXR are byte-identical: both implement the same shadow rays,
  explaining no visible toggle difference with Enhanced Shadows already on.
  This verifies hardware shadow tracing, not GI/reflections; separate-device
  readback migration remains unfinished.
- Connected-grid preparation benchmark now exercises 256 camera positions.
  EX straight view measured 5.38 us/update, 3205 average payload bytes and
  0.73 candidates/row (maximum 12); rotated view measured 19.63 us/update,
  6759 bytes and 5.12 candidates/row (maximum 13). Both stereo diagnostics pass
  (tmp/vr-connected-preparation[-straight]). These are CPU packet-preparation
  measurements only, excluding upload/GPU execution/presentation, not FPS gains.

### Quest stall investigation (September 11, continued)

- User requested removal of the immersive ground grid. VR world assembly now
  omits its packet entirely; desktop/native diagnostics retain their grids.
  Original and EX tests verify painter order and compute packet-index remapping.
- Device log measured a 25,883.8 ms first-frame scene upload/setup, with
  616.412 ms model preparation and 575.203 ms layer preparation. No stereo
  frame was submitted in that run. This establishes a startup stall, not the
  cause of every reported gameplay slowdown.
- Startup/runtime menu now skips invisible game scene preparation. Removed
  unused test-triangle pipeline creation from gameplay; packet passes compile
  only topologies they use. Added slow shader compilation and per-stage CPU
  timing logs. That build was installed, but controller-required launch UI
  prevented fresh performance verification.
- Ordinary models now defer unused clipping/span compute pipeline creation
  and descriptor binding. Ordinary/effect/ordinary transitions dispatch in
  the Vulkan test, with existing stereo coverage checks passing.
- Model updates compare retained input blocks and upload only changes. A
  real Vulkan fixture measures 2,100 bytes for full inputs, zero for identical
  inputs, and 480 for pose-only changes. Palette bypass invalidation and
  restoration pass. Proof: tmp/vr-input-byte-proof. These latest compute/input
  changes are not yet headset-verified; no FPS improvement is claimed.
