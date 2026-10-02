# VR wrap-around background inventory

## Subsequent fixes

Level 3 Corneria: BG_3_1C was absent from the landscape mapping. Source BGS.ASM
lines 116 onward select the same `stp` characters/map, ground mode and Y scroll
232 as BG_1_1C, but a different palette. Added the missing symbol for both
cartridges. Original front/rear stereo captures pass and left eyes were visually
inspected: tmp/vr-corneria3-after and tmp/vr-corneria3-rear (live-scene-*.bmp).
EX fresh 60-tick inventory now selects landscape for ID 45. EX visual capture
and headset acceptance remain. The table below records the pre-fix inventory.

September 12: ran `--preflight ROM SYMBOLS LEVEL 600 --background-audit` for
every numbered LEVEL1..7 symbol in Original and EX. Every invocation completed.
This samples 600 source ticks without input, not a full playthrough; deaths can
end gameplay early. It inventories mapping selection, not rendered correctness.

Original entries still using the flat mapping:

| Stages | Background ID | Mode |
| --- | ---: | ---: |
| 1-3 | 39 | 2 |
| 1-4 | 81 | 2 |
| 1-5, 2-5 | 87 | 2 |
| 2-6 | 165 | 1 |
| 3-1 | 21 | 2 |
| 3-2 | 183 | 2 |
| 3-6 | 225 | 2 |

EX flat entries: 1-2 (57), 1-3 (69 then 93 at tick 272), 1-4 (135),
1-5/2-5 (141), 2-4 (231), 2-6 (237, Mode 1), 3-1 (45), 3-2 (255),
3-6/4-3 (297), all 5-x (309/321/345/351/357), all 6-x
(363/369/375/399/411/417), and all 7-x (465/477/483/489/495).
Other listed EX entries are Mode 2. Unique-planet mappings also need visual
inspection; their classification alone does not prove complete surrounds.

Transitions requiring explicit tunnel acceptance:

- EX 4-4: landscape 291 -> tunnel 105 at tick 180 -> Mode-1 flat 237 at 321.
- EX 4-5: landscape 303 -> Mode-1 tunnel 153 at tick 345.
- Original 3-5 changes to game-over/Mode 1 at tick 557; that is not a new
  gameplay background and must not be mistaken for a missing landscape.

Next: inspect each distinct atlas before choosing star sphere, landscape,
unique panorama or enclosed tunnel. Do not blindly repeat planet/art panels.
Capture front/side/rear views and transitions, then test on the headset.

## Subsequent verified Original 1-4 mapping

Original BG_1_4 (81) now selects a landscape surround at atlas origin 232.
Its authored atlas contains unique planets only in the left 256 pixels;
outside the forward occurrence the shader samples the planet-free right half.
EX's same-named background remains excluded until its distinct atlas is checked.
Front and rear stereo captures passed in `tmp/vr-stage14-wrap` and
`tmp/vr-stage14-wrap-rear`; inspected left-eye images show continuous sky and
ground, with no repeated planet behind the viewer. These are host Vulkan
captures, not headset acceptance. A stage diagnostic asserts the mapping.

Additional atlas evidence is in `tmp/vr-bg-audit-LEVEL*`: Original 1-3 has a
planet in the upper-right quadrant; 3-2 has one in the lower-left. Neither
can be repeated wholesale. Original 1-5 and 3-6 share a starfield/green orbital
horizon. Original 2-6 starts with the colony Mode-1 water cross-section,
not the ordinary INATUNNEL=1 classification; its surround needs separate
handling. These entries are still pending, not counted as completed.

## Subsequent Original orbital horizon conversion

Original BG_1_5 (87, also used by 2-5) and BG_3_6 (225) now use a
surrounding orbital horizon and a separate star sphere sampling only the
verified star-only upper 256 atlas rows. Clear colour uses the border palette,
eliminating the brown fallback beyond the native window. EX stays excluded
pending inspection of its artwork. Front and rear stereo captures for 1-5
and rear captures for 3-6 are in `tmp/vr-orbital-LEVEL1_5-60`,
`tmp/vr-orbital-LEVEL1_5-60-rear`, and `tmp/vr-orbital-LEVEL3_6-60-rear`.
The inspected left-eye images show stars and the continuous green orbital
horizon at front and rear. Host Vulkan checks pass; no headset acceptance yet.

## Planet-bottom correction (supersedes orbital star-surround description)

Following the user's clarification, LSB orbital scenes now fill the lower
hemisphere with the planet surface, not stars beyond a narrow horizon strip.
`orbital_planet_sphere_packet` maps the atmosphere at atlas row 384 and the
surface band through row 464 to the nadir; the sky occupies the upper half.
Native scroll/scanline deformation is disabled for this enclosing geometry.
Front/rear/downward stereo host captures pass in `tmp/vr-planet-bottom-*`;
inspected left-eye images have no stars below the planet horizon. The nadir
still has polar texture convergence and needs visual refinement/headset review.
Packet tests assert every lower-hemisphere vertex samples the surface band.

EX 1-4, 1-5 and 3-6 authored atlas BMPs are SHA256-identical to Original at
60 source ticks (`tmp/vr-ex-atlas-LEVEL*`). Enabled their matching landscape
and orbital mappings; EX remains subject to transition/headset acceptance.

## EX authored-atlas inspection

Captured all numbered 5-x, 6-x and 7-x entries at 60 source ticks, plus
1-2/1-3/1-4/1-5/2-4/3-2/3-6, in `tmp/vr-ex-atlas-LEVEL*`.
All capture invocations completed; each atlas was visually inspected.

- 1-2: repeatable asteroid dust/stars; 2-4: repeatable nebula/stars.
- 5-3: repeatable blue nebula/stars; 6-3: repeatable asteroids/stars.
  These four now select star spheres.
- 7-1/7-2/7-3/7-4: repeatable clouds, mountains/desert/snow and ground,
  with horizon near atlas row 344. Added landscape mapping at origin 232.
- 5-1/6-1: landscape with isolated moon in the right half; preserve once.
- 5-2: city horizon plus isolated celestial artwork; needs separated mapping.
- 5-4: clouds/ground plus two planets; retain existing unique-ink protection.
- 5-5/7-5: fiery band and isolated bright object; needs separated mapping.
- 6-2: water horizon and isolated island; avoid repeated island.
- 6-4: ground with a large sky opening and two bright orbs; preserve once.
- 6-5: dark sky/city strip; 6-6: ground/clouds with concentrated fiery feature.
  These are not yet enabled merely because part of the artwork is repeatable.
- 1-3 and 3-2 retain their unique planets and still need full surrounds.

The new 60-tick scene assertions check the eight enabled star/landscape
selections. Mapping selection alone does not establish visual/headset parity.

All eight rear stereo capture invocations passed in `tmp/vr-ex-wrap-LEVEL*-rear`;
left-eye captures were inspected. The four starfield scenes fill the rear view,
and all four landscapes have surrounding sky/ground. 7-1 additionally shows
repeated white shapes near the horizon in the combined scene; isolate its grid,
models and background before calling that view visually accepted. 7-2/7-3/7-4
have no corresponding white-shape artifact in these captures. Fresh packet
tests passed. Forward/side views, transitions and headset checks remain.

7-1 isolation: added diagnostic `@background-only` (combinable before `@rear`)
to the host scene capture. `tmp/vr-ex71-background-only` still shows the white
horizon wedges with all models, grid and sprites omitted. Both stereo renders
and the diagnostic suite completed. This localizes the artifact to the
background path, not gameplay object duplication. Next inspect/interrogate
landscape UV interpolation where the spherical sky meets the flattened ground;
do not treat the combined-scene model or grid paths as the cause without evidence.

## Resolved sky/ground horizon wedges

Perspective-UV ablation produced byte-identical captures: the fragment entry
already selects perspective UVs for tile geometry. That hypothesis was rejected
and the experimental shader edit reverted.

Confirmed cause: `place_landscape_ground` moved the duplicated horizon vertices
belonging to sky triangles from sphere radius 64 to the floor's far edge (4096).
This stretched the adjoining cloud triangles into wedges. Flattening now operates
per triangle and preserves every vertex of an upper-sky triangle, including its
horizon copy. Lower floor triangles keep their own flattened horizon copies.
`tmp/vr-ex71-horizon-split` rear stereo captures pass; inspected left eye shows
the cloud band without white wedges. Packet regression checks both independent
horizon copies and preserves immutable source geometry. Fresh packet tests pass.
Headset verification remains pending.

## EX twin-planet surround verified on host

BG_5_4 now selects landscape projection while retaining the existing per-ink
planet-repeat suppression. Fresh `tmp/vr-ex54-surround-{front,left,right,rear}`
captures completed for both eyes; all four left-eye views were inspected.
The front contains both authored planets; side/rear views have repeating
clouds/terrain without extra planets. Packet regression requires the unique
planet shader flag to survive landscape conversion. A 60-tick EX stage
assertion requires both landscape and twin-planet policies. These captures use
the corrected native camera and 256-pixel angular scale. Physical acceptance
and later background transitions remain pending.

## In-progress unique space planets

BG_1_3I and BG_3_2 now select one full atlas occurrence, surrounded by a
planet-free upper-left star patch. Payload/shader/validator support explicitly
allows a 512-row unique atlas and quadrant sampling; other unsupported row
counts remain rejected. Packet tests and three host stereo capture suites pass.
`tmp/vr-unique-space-LEVEL3_2-60-rear` shows stars without a repeated planet.
However, the down/up views expose severe planet distortion on spherical UVs
(`LEVEL3_2-60-down`, `LEVEL1_3-60-up`). This is NOT visually accepted.
Next replace the unique atlas sphere overlay with a distant proportion-preserving
planet patch; keep only the star layer spherical. Do not ship this intermediate
mapping as a finished fix. This latest mapping is not deployed on Quest.

Subsequent correction: unique planets now use a six-vertex tangent patch at
distance 64, not a textured spherical atlas. Rectangle coordinates are assigned
per verified atlas (BG_1_3I and BG_3_2), and the surrounding stars retain their
planet-free quadrant. `tmp/vr-planet-patch32` down-view capture passes and shows
a rounded planet edge instead of the previous narrow stretch, though the planet
is partly outside that view. Packet tests verify orthogonal equal-length sides
for a square source rectangle and a center distance of 64. Full centered views,
source-scroll continuity, EX visual coverage and headset acceptance remain;
do not infer all of these from the partially visible planet capture.

Focused-view acceptance: `@planet-focus` aims the diagnostic eye at the actual
six-vertex patch center. Original and EX 1-3/3-2 stereo captures all pass in
`tmp/vr-planet-focused-LEVEL*` and `tmp/vr-ex-planet-focused-LEVEL*`.
All four left-eye images were inspected: complete round planet outlines,
not the earlier spherical stretch. This resolves the full-view shape check;
source-scroll continuity, transitions and headset acceptance are still pending.

Scroll correction: planet placement now consumes the effective signed scroll
from the background payload, including `scroll_override`, instead of reading
raw PPU scroll registers directly. Regression compares a negative-X/positive-Y
override with equivalent register values and requires identical geometry;
fresh packet tests pass. Temporal interpolation/wrap-boundary behavior still
needs separate verification and is not established by this equivalence test.

## Colony enclosure correction

BG_2_6A uses source WATER mode rather than INATUNNEL. The VR presentation now
marks this exact resolved background as enclosed without changing source RAM
or treating open Titania water as a tunnel. The host composite capture now
also draws the production tunnel enclosure after models and before HUD sprites.
Original LEVEL2_6 at 60 ticks: inspected front/left/rear captures in
`tmp/vr-colony-surround-*`. Front retains the center opening; left/rear are
solid black. The rear Vulkan suite passes with an explicit zero-lit-pixel
assertion (a blank rear is required here, not a model-rendering failure).
The earlier left invocation tripped the generic blank-image check; its image
was inspected but that invocation is not counted as a passing suite.
EX, further colony transitions and physical headset coverage remain pending.

## EX 5-1 / 6-1 landscape surrounds

The atlas diagnostic previously cropped every background to 512 pixels. It now
uses the actual BG2 atlas width, including 16-pixel tiles. These two scenes
use 1024-pixel-wide atlases; inspected full-width output confirms the small
moon at x424 and moon-free first 256 pixels. Added landscape classification
with one authored 0..512 occurrence and moon-free sampling outside it.
The combined 64/128 coverage flags select this policy; 128 alone still means
quarter-atlas star sampling. Packet builder and Vulkan validator agree on
supported 512/1024 widths. Packet regressions cover both tile sizes.

Initial right-view captures exposed the moon stretched into a vertical line:
clamping upper sky UV to row 232 replicated its edge indefinitely. The new
mapping extends to verified clear row 200 instead. Final isolated right/rear
stereo suites pass for both stages in `tmp/vr-moon-final-LEVEL*-*`; all four
left-eye images inspected: small moon on the right, no moon in the rear,
continuous sky/cloud/ground coverage and no former vertical moon stripe.
These are 60-tick host Vulkan captures, not full-level or headset acceptance.
Changes remain local and are not yet installed on Quest.

## EX 6-5 night-city surround

Inspected the full native-width atlas in `tmp/vr-ex65-full`: uniform dark sky,
repeatable city strip and black below, no isolated celestial landmark. Source
scroll is 232. BG_6_5 now selects landscape mapping at that same origin.
Packet tests pass; isolated front/right/rear stereo suites pass in
`tmp/vr-ex65-wrap-*`, with all three left-eye images inspected. These show
continuous dark sky and city/black lower view rather than the earlier gray
fallback outside a rectangular scene. Full-level and headset acceptance remain.

EX 6-2 full-width evidence (`tmp/vr-ex62-full`) reveals a 1024-pixel atlas with
islands repeated at x~280 and x~792. The earlier 512-pixel dump concealed that
duplication. Its first 256 pixels also overlap an island edge near the horizon;
do not reuse the moon-free-quarter rule without selective island handling.
6-2 remains unconverted, not accepted as complete.

## EX 6-2 ocean conversion in progress

BG_6_2 now selects a surrounding landscape. A dedicated payload flag remaps
only rows 344..359 outside the first authored 512-pixel occurrence to the
verified open-water first 128 pixels. The remaining cloud/sea atlas is left
intact. Unique-row decoding masks out this separate flag. Packet tests verify
flag selection and that it is absent without expanded-horizontal options.

Initial four-direction stereo suites pass in `tmp/vr-ex62-island-*`, but sky
clamping stretched the top cloud row upward. Extended this scene into clear
atlas row 168; fresh front/rear suites pass in `tmp/vr-ex62-final-*`, and both
left-eye images were inspected: continuous ocean/sky, no vertical cloud smear.
The retained island is NOT clearly identifiable in these low-resolution views.
Do not call island preservation accepted until a focused/high-resolution view
demonstrates it; horizon projection may be compressing the landmark. Full-level
and headset acceptance also remain. These changes are local, not deployed.

## Ocean island horizon correction

Added a narrow-FOV `@island-focus` host view aimed at atlas x296. Before capture
`tmp/vr-ex62-focused-before` shows no island even when enlarged. The atlas's
shoreline is row 352 rather than the usual 344, so the old receiver mapping
flattened the island. BG_6_2 now uses origin 240 (112+240=352). Focused after
capture `tmp/vr-ex62-focused-after` passes and clearly shows the island above
water; both before/after left-eye images were inspected. This supplies direct
evidence for the previously unresolved preservation issue, not just a flag test.
Full-level transitions and headset acceptance remain outstanding.

Fresh four-direction stereo suites with this corrected origin pass in
`tmp/vr-ex62-shoreline-*`. All four left-eye images inspected: the forward
island edge is retained and the rear/side surrounds have continuous sea and
sky, without an additional island. This remains a single 60-tick scene sample.

## EX 6-4 unique sky opening

Full native-width captures of remaining scenes are in `tmp/vr-remaining-full-LEVEL*`.
All five suites (5-2, 5-5, 6-4, 6-6, 7-5) pass; atlases inspected. 5-5/7-5
atlas hashes are identical at this sample. They and 5-2/6-6 still need mapping.

6-4's 1024-pixel atlas places the opening/orbs only in its left 512 pixels;
the right half has repeatable sky/ground. BG_6_4 now selects unique-left
landscape coverage with atlas origin 240 and clear upper-sky extension.
Shader half selection derives from actual tilemap width, retaining the old
256/512 behavior and adding 512/1024 behavior. Packet tests cover the larger
atlas; Vulkan validation still rejects a 256-pixel unique-half layout. Its old
1024 rejection fixture was updated because that layout is now valid.

Front/right/rear/left stereo suites pass in `tmp/vr-ex64-opening-*`.
All four left-eye views inspected: the forward sky opening and both orbs are
retained, and side/rear surroundings use clear sky/ground without duplicates.
This verifies a 60-tick host sample, not full-level transitions or headset parity.
Not yet deployed to Quest.

## EX 5-5 / 7-5 fiery surrounds

The captured atlases are byte-identical (SHA256
4236BFB20F0DB29D007C954D1CEA6A4F66870AACFA43618F0D9EEC1CE0B614B2).
Both backgrounds now use unique-right landscape coverage: retain the first
512-pixel occurrence, repeat only the bright-object-free first 256 pixels
outside it. Origin 248 places the ground boundary at atlas row 360 and the
upper clamp at clear red row 216, avoiding tan filler above the sky.
Packet tests pass. Six front/right/rear stereo suites pass in
`tmp/vr-fire-LEVEL*-*`; all six left-eye views inspected. The bright object
appears once in front, not at the sides/rear; red sky, flame band and ground
continue around the viewer. This proves sampled 60-tick host presentation,
not every animation phase/transition or headset acceptance. Not deployed yet.

## EX 6-6 volcanic surround

BG_6_6 uses a landscape origin of 240 (ground boundary 352). Its dedicated
coverage flag replaces only rows 320..351 outside the first authored 512-pixel
occurrence with the eruption-free first 128 pixels. Other cloud/ground artwork
keeps its native sampling. The upper sphere extends to clear atlas row 64,
avoiding upward stretching of the cloud row at the native screen edge.
Packet tests cover flag encoding, unique-row separation and the upper UV limit.
Four-direction stereo suites pass in `tmp/vr-ex66-volcano-*`; all four left-eye
images inspected: one bright eruption ahead, none behind or at the sides,
continuous dark clouds and red ground. This is a 60-tick host sample only;
animation phases, transitions and headset acceptance remain. Not deployed yet.

## EX 5-2 city surround in progress

BG_5_2 now uses origin 248 and unique-right landscape coverage. Dedicated
sampling extends the planet-free star band overhead and uses ordinary city
art outside the first 512-pixel occurrence rather than repeating the palace.
Packet tests cover flag separation and upper-hemisphere UV coverage.
Initial overhead capture exposed a large empty cap (`tmp/vr-ex52-city-up`);
angular upper-sky UVs remove it (`tmp/vr-ex52-zenith-fixed`, inspected).
Fresh front/right/rear stereo suites pass in `tmp/vr-ex52-final-*`, with all
left-eye images inspected: palace only ahead, planets on the right only,
surrounding city/stars and no rear duplicates. However, the larger planet
appears vertically elongated in the right view. This mapping is NOT fully
accepted: use a proportion-preserving unique patch or otherwise verify/correct
planet geometry before claiming this scene complete. No Quest deployment yet.

## EX 5-2 planet proportion correction

The sphere now replaces the planet atlas region with ordinary stars and draws
the authored planet group once using the existing distant tangent-patch path.
Rectangle {384,208,56,48} excludes the non-art filler above row 208. An initial
{384,192,56,64} capture exposed a filler strip and was rejected. Application
now applies the same interpolated landscape camera to both background packets,
so the patch cannot remain stationary while the surrounding sky moves.

Packet checks and four final stereo suites (focused/right/rear/up) pass in
`tmp/vr-ex52-patch-final-*`; all left-eye captures inspected. The larger planet
retains its authored proportions, the filler strip is gone, and no planet
duplicates or overhead empty cap are visible. These captures resolve the
previous sampled visual rejection. Full-level animation/transition and physical
headset acceptance are still outstanding; this change is not deployed yet.

## EX enclosure transition samples

Fresh front/rear captures at LEVEL4_4 ticks 180/330 and LEVEL4_5 tick 360
all pass their stereo suites (`tmp/vr-transition-*`, `tmp/vr-transition-front-*`).
All six left-eye images inspected: front gameplay remains in the source opening,
rear is solid black with no leaked models. Sampled IDs/modes are 105/2,
237/1 and 153/1, exercising both ordinary tunnel and colony classification.
These are isolated checkpoints, not a temporal transition smoothness proof.

The 4-4/180 front view contains a noisy vertical strip inside the opening.
Background-only ablation in `tmp/vr-transition-ex44-bg-only` passes and is
captured separately for investigation; do not describe the complete scene as
visually accepted based only on enclosure correctness.

Inspected the background-only image: the noisy vertical strip disappears when
models/grid/sprites are omitted. The artifact therefore is not demonstrated in
the BG2 layer; next isolate the omitted draw layers and their texture payloads.

## EX 4-4 layer isolation

Added host-only `@no-models` / `@no-sprites` ablations. Both stereo suites pass
at 180 ticks (`tmp/vr-ex44-no-*`); inspected left eyes show the strip remains
without models and disappears without the combined sprite pass. That pass
also includes EX's BG1 bitmap, so this does not isolate OAM alone.
The diagnostic now exports `source-sprites-reference.bmp` using the CPU OAM
decoder from the identical PPU snapshot. `tmp/vr-ex44-sprite-reference` passes;
the inspected CPU reference has ordinary bomb icons and no noisy strip.
Next distinguish the BG1 bitmap overlay from GPU OAM before changing sprite
decoding or suppressing legitimate EX text. The visual defect remains unfixed.

Bitmap isolation: added `@no-bitmap` for stage captures without triggering the
unrelated space-stage warmup. `tmp/vr-ex44-no-bitmap` passes and its inspected
left image is clean while ordinary sprites/HUD remain. Added a CPU BG1 export
using the same 16-pixel guard and transparent-black rule as production desktop.
`tmp/vr-ex44-bitmap-reference/source-bg1-reference.bmp` contains the same noisy
strip. Therefore the defect is present in source bitmap/VRAM presentation data,
not uniquely caused by Vulkan tile decoding. Next investigate
Wdc65816::begin_superfx_bitmap_frame / submit_superfx_bitmap and source transfer
state around this tunnel transition. Do not suppress BG1 globally: EX dialogue,
pause and tally depend on it. No production fix for the strip has been made.

Transfer-state evidence: captures at EX 4-4 ticks 160/180/200 all pass in
`tmp/vr-ex44-bitmap-state-*`. Logged M_CLRBITMAPS=1 throughout; BG1 character
base=0, tilemap=11264, scroll=(0,0), VMAP1=12288 and VMAP2=0 at all three
sampled boundaries. The scene changes from BG291 to BG105 between 160/180.
This contradicts a simple disabled-clear-flag explanation and provides no
evidence of a wrong buffer swap at those boundaries. Next instrument writes
to BITMAP1 after begin_superfx_bitmap_frame (including decompression/native
draw operations), rather than adding an unconditional post-draw clear that
would erase legitimate EX text.

Byte-level evidence at EX 4-4/180 (`tmp/vr-ex44-bitmap-bytes`): BITMAP1 RAM
contains 4096 nonzero bytes; the displayed BG1 character VRAM range also has
4096, with zero byte mismatches across all 21504 bitmap bytes. The corruption
therefore predates DMA. Added read-only RAM/VRAM counts to the diagnostic.
EX symbols resolve DEC_BASE=$2800 and BITMAP1=$4000; the host calculates
screen_decrunch_buffer as DEC_BASE+6144, also $4000. This is a concrete overlap
to investigate against the cartridge's transfer/clear ordering, not yet proof
that the source intends a different address. Do not change the offset or erase
the bitmap without identifying the responsible write and source semantics.

## EX bitmap scratch collision fix

Temporary checksum tracing around native calls identified DOBGREQ_L ($03edc8)
as the writer (`tmp/vr-ex44-write-trace-writes.log`). Source BGS.ASM dec_bg_l
uses DEC_BASE+6144; EX aliases that with BITMAP1. The host services the request
after preparing the current frame overlay, then publishes the overwritten RAM.
GameSimulation now preserves/restores that bitmap across background requests
only for EX gameplay/training. The loader still performs its normal DMA; native
overlay ink is retained rather than globally cleared or hidden. Temporary
per-call tracing was removed after diagnosis.

Fresh captures at 180/181/200 ticks pass (`tmp/vr-ex44-bitmap-preserved`,
`tmp/vr-ex44-preserved-*`). All left-eye images inspected: no noisy strip,
ordinary HUD and bitmap pass remain enabled. RAM/VRAM report zero nonzero bytes
and zero mismatches for these legitimately empty overlays. Rebuilt Original/EX
game-input and packet tests pass 3/3. Still verify a simultaneous nonempty
dialogue/debug overlay during a background request and physical headset play;
these empty-overlay samples alone do not prove text retention in every case.
Fix remains local, not yet deployed.

## Nonempty EX overlay retention

Added `@scored-overlay`, enabling the cartridge's SCORED mode in the host
fixture. CESTIMER_L writes its real SCORE label/value before the late background
request, exercising preservation of legitimate ink rather than an empty bitmap.
Captures at ticks 160/180/181 all pass (`tmp/vr-ex44-scored-*`); inspected left
eyes retain SCORE before/during/after the transition, without the noisy strip.
Each reports 208 nonzero RAM bytes, 208 nonzero displayed VRAM bytes and zero
byte mismatches. The diagnostic now requires nonempty ink and exact RAM/VRAM
transfer whenever this option is selected. Other overlay types and headset
acceptance remain separate checks; no new headset deployment in this pass.

The new assertion was rebuilt and passed in `tmp/vr-ex44-scored-assert`.
Quest ARM64 APK rebuild succeeded (33 seconds); this includes the recent EX
surround mappings and bitmap scratch preservation. Build success is not
headset acceptance. Earlier 'not deployed' notes describe their original
verification state, not a permanent exclusion from subsequent builds.

Installed that APK on authorized Quest 3 2G0YC1ZF8R059K using install -r;
package manager returned Success. No game process was running before install.
Saves/settings retained, no automatic launch and no physical acceptance claim.

## Extended-sky geometry cache

The four extended-sky UV variants now lazily cache immutable geometry for each
target color space at the standard horizon. Thread-safe call_once initializes
only variants actually used; custom horizons retain independent generation.
This removes one vector copy/allocation and 12,288 per-vertex UV calculations
per repeated landscape preparation (ground-height deformation still runs).
Tests verify same-variant sharing and separation across variant/color space.
Packet checks pass. Three fresh stereo capture suites pass in `tmp/vr-sky-cache-*`;
all six BMP hashes exactly match the earlier city-up, volcano-front and opening-
rear baselines. No measured in-game FPS claim; this is eliminated CPU work with
byte-identical sampled output. Not included in the preceding Quest install.

## Later Asteroid background variant

EX LEVEL1_3 at 300 ticks selects BG_1_3C (ID93), which was still using the
flat fallback. Its full atlas is byte-identical to the entry BG_1_3I capture
(SHA256 62E6E7DB6F314AE94ECA27658A3C560AB1D8DC5E3FB82973929F947E67A87A30).
Added BG_1_3C to the unique-space mapping, using the same planet rectangle and
planet-free star quadrant. Original source BGS.ASM also assigns C the same
characters/map as I; Original's 300-tick sample remains I, not C.
Fresh EX focused/rear stereo suites pass in `tmp/vr-ex13-late-fixed-*`, and
both left eyes were inspected: complete planet ahead, stars without another
planet behind. Packet checks pass. Other variants and full transition motion
still require verification; this addition is not in the prior Quest install.

## Asteroid normal-flight mapping

Added BG_1_3A to the same single-planet surround as I/C. Original BGS.ASM
explicitly selects chr34/scr34, palette space and scroll232 for all three;
BG_1_3B remains separate because it selects the Mode1 tunnel artwork.
Host scene/packet/input targets rebuild successfully and Original/EX input
plus packet CTest checks pass (3/3). The fresh Original 120-tick rear capture
in tmp/vr-asteroid-flight-a passes stereo checks and was inspected, but its
reported background is still ID39 (I), so it is a regression capture, NOT
direct visual proof of the A transition. A-specific runtime artwork/transition
verification, including EX, remains pending. Not deployed to Quest yet.

Deployment update: rebuilt the Quest development APK successfully and installed
it with install-r on authorized Quest3 2G0YC1ZF8R059K (Success). The package
includes the extended-sky geometry cache and Asteroid A/C mappings added since
the preceding install. No game process was running before installation; saves
and settings retained. No launch or physical rendering acceptance claimed.
# Late EX transition audit (September 12)

BG_5_1I entry mapping is now implemented separately: planet surface rows
424..464 fill the lower hemisphere; the small planet rectangle336,320,56,64
is removed from repeating atlas samples and drawn once on a distant patch.
Patch vertical placement shares the424-row horizon (scroll312). Fresh
tmp/vr-orbital-entry-rear and tmp/vr-orbital-entry-planet-focus stereo checks
pass; inspected rear has no small planet, focused view has one above horizon.
Lower-surface stretching remains visible and belongs to the unresolved
orbital projection work; not headset-approved or installed.

Rejected experiment: `tmp/vr-orbital-detail-down` repeats the thin surface
strip with mirrored shader sampling. Though packet/Vulkan checks pass, the
inspected image produces strong repeated atmospheric bands. The experiment
was removed, retaining the previous cap. Simple mirrored-strip repetition is
not an acceptable solution; separate atmosphere from surface continuation.

Orbital nadir follow-up: geometry now transitions from cylindrical horizon
UVs to a Cartesian cap below the horizon. All pole vertices agree on UV,
covered by packet regression in both colour spaces; immutable cache retained.
Fresh Vulkan captures `tmp/vr-orbital-cap-down`, `tmp/vr-orbital-cap-front`
(EX BG_5_1E), and `tmp/vr-orbital-cap-original-down` pass. Inspected nadir
images no longer have the single-point spoke convergence. However the EX
15-pixel surface remains visibly stretched, including in the forward view.
This is intermediate projection work, not acceptable final orbital visuals,
and has not been installed. Improve surface continuation before handoff.

BG_5_1E now has a separate orbital mapping restricted to surface rows
400..415, rather than LSB's 384..464. Both colour-space variants have UV-bound
and immutable-geometry-reuse regression coverage; packet checks pass.
`tmp/vr-thin-orbital-fixed-down` is the fresh rebuilt stereo capture and passes
Vulkan checks. The earlier `tmp/vr-thin-orbital-down` ran the old executable
during a Windows link lock and is NOT evidence for this change.
The fresh downward image correctly contains planet instead of stars, but
exposes severe polar texture convergence. This remains visually unresolved;
do not call the orbital wrap finished or headset-approved. Not installed.

The transition diagnostic now selects EX simulation behavior explicitly when
loading an EX cartridge. Forty numbered EX stages completed bounded 4,000-tick
traces; these are not full playthroughs or boss-defeat coverage.

Added BG_6_3H to the repeatable star/asteroid sphere mapping after inspecting
its actual 1,024x512 atlas. `tmp/vr-late-63h-wrapped` captures LEVEL6_3 at
2,100 ticks looking rearward; stereo Vulkan checks pass and the rear image
contains the authored asteroid field rather than an empty fallback.

Verified the new BG_1_14 landscape mapping with `tmp/vr-ex114-wrapped`
(LEVEL1_4, 60 ticks, rear). Stereo checks pass; inspected rear view has
continuous ground and stars without the unique front planet repeated behind.
Neither capture is headset acceptance. These mappings are not yet installed.

Still unresolved: BG_5_1I contains both a unique small planet and a lower
planet horizon. BG_5_1E has a much thinner horizon strip than the existing
BG_1_5 orbital mapping, with stars below it. It must not reuse that mapping's
384..464 sampling range, which would incorrectly fill the lower view with
stars. Their inspected source atlases are in `tmp/vr-late-LEVEL5_1-60` and
`tmp/vr-late-LEVEL5_1-2300` respectively.
