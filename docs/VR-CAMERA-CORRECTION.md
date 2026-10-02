# Active VR camera correction

Latest headset report supersedes acceptance of host-only background captures:

- Vertical steering must keep the player centered.
- Objects and grid must remain attached to ground through that motion.
- Background is visibly squashed.
- Mothership exit must preserve the scripted camera movement.

Confirmed current implementation problems:

1. `interpolate_scene_poses` replaces interpolated camera Y with the landscape
   entry height whenever `fixed_landscape_height` is enabled. This cancels the
   source tracking translation, including the translation needed to follow the
   player vertically. `assemble_grid_interpolated` repeats that policy.
2. The same policy replaces the entire native view matrix with
   `landscape_scene_view`, derived from background scroll and roll. It cannot
   preserve scripted source yaw/pitch during departure.
3. Landscape UV generation uses `horizon - y*512/radial`, clamped to 0..223;
   most texture detail is concentrated around the horizon. Entry-height ground
   flattening further assumes a stationary camera. This needs a projection
   design checked in-headset, not acceptance based only on small host captures.

Next implementation must retain a shared authoritative camera for player,
objects, grid and the ground receiver. Camera-height movement must change the
receiver's camera-relative height, not detach the objects or suppress player
tracking. Preserve scripted source rotation; distinguish any intentional
steering-only comfort adjustment from cinematic camera motion. Validate
vertical steering at both extremes and the full departure camera sequence.

Do not fix player centering with a player-only screen offset. Do not continue
marking individual atlas mappings visually accepted while the common camera
and angular projection remain wrong. Previously completed mappings and tests
remain implementation evidence only, not headset acceptance.

## Implemented correction, awaiting runtime acceptance

Removed fixed-height cancellation from model/grid interpolation and restored
native interpolated view matrices. Landscape receiver geometry now captures
current camera height; `landscape_camera_motion` applies the same native view
rotation plus the between-tick receiver-height correction. Background angular
mapping now uses the source 256-pixel projection scale instead of the extra
2x compression. Synthetic pose tests verify camera-relative movement and native
pitch retention; receiver interpolation has a matching-height regression.
Host build and initial packet tests pass. Full steering/departure capture and
headset acceptance remain required; this has not yet been deployed to Quest.

Deployment update: fresh Quest ARM64 debug build succeeded (15 seconds) and
`adb install -r` on authorized Quest 3 `2G0YC1ZF8R059K` returned Success,
preserving data. Launch was intercepted by the headset's
`LaunchCheckControllerRequiredDialogActivity`; the game did not obtain a
verified launch. Wake/connect controllers and resolve the headset prompt for
visual acceptance. Host Original Corneria capture in
`tmp/vr-camera-native-corneria` passed the Vulkan diagnostic suite and was
visually inspected. It does not prove the user's steering/departure cases.

Expanded regression: ground-attached object poses and the independently
transformed receiver point agree in all three axes across 13 interpolation
phases for pitch, yaw and roll changes while camera height changes. Fresh
packet tests pass. Receiver interpolation now also snaps at flow transitions,
matching the object interpolation guard. This is mathematical alignment
evidence, not a substitute for the full mothership departure or headset test.

## Live steering disproves complete centering

Added host stage suffixes `@steer-up` / `@steer-down`, using the input latch
only after normal checkpoint warmup. Captured Original LEVEL1_1 at 120 source
ticks with held Up and Down (`tmp/vr-camera-steer-up`, `tmp/vr-camera-steer-down`).
Both Vulkan suites pass, but inspected images show the ship at different
vertical screen positions. Source camera Y is -49 versus -204. Native camera
restoration alone therefore does NOT fulfill centered VR steering; do not
report this as fixed based on the synthetic alignment tests.

Next: derive a shared VR tracking camera from the player's movement while
retaining scripted departure-camera motion. Apply it consistently to objects,
grid, ground and effects; avoid a player-only screen offset. Distinguish native
steering camera behavior from cinematic strategy motion using source state.

## Normal-flight tracking correction

Source PSTRATS.ASM explicitly follows 75% of player Y on planets and 62% in
space relative to the flight center. Added presentation-only compensation
`player.worldY - PVIEWPOSY` to the shared scene camera for exactly
PLAYERONPLANET_STRAT and PLAYERINSPACE_STRAT, resolved per cartridge. Camera
shake/orbit remains; departure, death and other strategies do not select this
correction. No source RAM or player-only draw position is changed. The receiver
height is captured after the correction so it follows the same camera.

Fresh Original up/down captures at 120 ticks in `tmp/vr-camera-fulltrack-*`
both pass host Vulkan checks. Inspected images show the ship near the same
vertical center while the environment moves; camera Y is -40 versus -260,
instead of -49/-204 with partial native tracking. Packet tests pass. EX live
steering, full departure continuity and physical acceptance remain pending.

EX steering captures now complete at both 120-tick limits in
`tmp/vr-ex-fulltrack-*`. Numerical diagnostic for held Down reports player
Y=-277, presentation camera Y=-276, view float=1, pitch=0. The ship anchor is
therefore centered to the remaining source float unit; high green reticle
marks must not be mistaken for the ship when reviewing the composite. Added
player Y/float/pitch/strategy output to host captures for explicit inspection.
Both Vulkan runs pass; physical centering and reticle placement remain subject
to headset acceptance.

## Departure capture from real stage entry

Added `@from-entry` to bypass the diagnostic checkpoint warmup, which previously
skipped the reported cinematic. Entrance fixtures may legitimately have no HUD
sprites; only those fixtures now allow an empty sprite layer. Captured EX
LEVEL1_1 at 60/120/180/240/300/360 ticks in `tmp/vr-departure-ex-*`.
All six stereo Vulkan invocations pass; all left-eye images were inspected.
60/120 show the mothership side and then the launch corridor from different
camera orientations (native pitch 54176 then 61456). The source strategy is
PLAYERPENING_STRAT (0x17d86d), outside the normal-flight tracking allowlist.
Later frames show the exterior/camera transitions. This demonstrates changing
rendered cinematic orientation, not full temporal smoothness or physical
headset acceptance; do not treat six still frames as a full-motion test.

## Quest test handoff (September 12)

Rebuilt the Quest development APK successfully and installed it with `adb
install -r` on authorized Quest 3 2G0YC1ZF8R059K (package manager Success).
Saves/settings retained; no automatic launch or physical acceptance claimed.
This build includes shared normal-flight vertical tracking, native scripted
camera rotation, corrected landscape UV proportions, unique planet patches
and the colony enclosure. Fresh packet checks and the existing Original/EX
game-input CTest executables pass (3/3). Headset checks still needed: held
vertical steering with ground objects, background proportions/tilt and the
full mothership departure sequence. The broader goal remains incomplete.
