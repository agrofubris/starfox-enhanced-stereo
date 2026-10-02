# Star Fox Enhanced 0.0.6.7

Alpha/prerelease checkpoint since 0.0.6.5. Platform packages attach automatically
as GitHub Actions completes each build. Publication is not a claim that every
platform build or physical-device check has completed.

## Rendering and performance

- Expanded native GPU model rendering, ordered tile/OBJ composition, clipping,
  geometry processing and resident presentation, reducing CPU rasterization,
  transfers and synchronization in supported paths.
- Moved more background and foreground layers to GPU processing, including
  title artwork, frontend margin filling, EX logo margin repair and the main
  planet-briefing background. Preserved CPU recovery paths.
- Added ScaleFX filtering and expanded portable GPU effects support.
- Added hardware ray-traced shadows on supported backends/devices, default off.
  Improved caster coverage, light direction and shadow composition; Enhanced
  Lighting remains a separate stylized effect, not ray tracing.
- Added half/full side-by-side 3D output and improved stereo layer composition.

## Gameplay, menus and presentation

- Added in-game options access with F1, with experience locked during play.
- Organized cheats under OPTIONS: god mode, level select, default laser/beam,
  infinite lives, bombs and boost.
- Added Ctrl+F1 save state, Ctrl+F2 load state and Ctrl+F3 slot selection.
- Improved regional title selection, including English (Europe), localized
  menus, controller handling and Android immersive fullscreen behavior.
- Updated widescreen background expansion, unique planet/artwork handling,
  tunnel margins, title layers, comms presentation and high-FPS transitions.
- Improved model/wireframe interpolation, audio state transitions and recovery.

## VR development

- Added Quest/OpenXR runtime work, immersive backgrounds, controller input,
  pre-game options, effects/preview support and asset-loading improvements.
- VR remains experimental; scene-specific presentation and performance still
  need further headset testing. Standard platform release jobs do not imply
  that a new Quest APK has been packaged or installed.

## Important limitations

- **DLSS and DLSS5 are not released player-facing features in this version.**
  Diagnostic integration remains default-off in source; no NVIDIA/third-party
  neural add-on binaries are bundled. Full-scene jitter, upscaling modes and
  neural gameplay integration are unfinished.
- GPU migration is not complete: isolated overlays, some late layers and sprite
  bitplane decoding still have CPU dependencies. Unsupported devices use
  available fallback paths; not all platforms support every enhancement.
- No copyrighted cartridge dumps, user saves or MSU music are included. Supply
  your own supported assets as described in the README.

## Verification at this checkpoint

- Windows runtime rebuilt during development. D3D12/Vulkan background and
  temporal-depth regression suites passed; generated shader checks passed.
- Original/EX main briefing captures match CPU and forced-fallback rendering
  at 16:9 and 32:9/4x with effects. Multiple earlier GPU migration checkpoints
  also include Linux/Android builds and comparison captures.
- See `docs/GPU-MIGRATION-STATUS.md`, `docs/GOAL-ACCEPTANCE.md` and
  `docs/PC-DLSS-STATUS.md` for exact tested scope and remaining work. This release
  does not close the full migration goal or automatically close GitHub issues.
