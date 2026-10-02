# Local visual proof — September 13

These are actual runtime captures, not mockups. Paths refer to this workspace's
private diagnostic outputs; no release or headset verification is implied.

## Live F1 → Options → Cheats

Fresh Original and EX captures using `check_runtime_options.ps1 -Cheats`.
Both were inspected: all six cheat rows and Back are readable and inside the
panel over the frozen gameplay scene. This proves submenu access/layout,
not every cheat's simulation effect or physical controller behavior.

![Original Cheats](../tmp/cheats-current-original/runtime-cheats.bmp)
![EX Cheats](../tmp/cheats-current-ex/runtime-cheats.bmp)

## EX 6-3: repeated face-planets

Before:

![Repeated planets](../tmp/background-audit-later-ex-current/EX-LEVEL6_3-6000-32_9.bmp)

After: surrounding stars remain; only duplicate planet/saucer regions are removed.
Native 4:3 and the native center of the wide image are byte-for-byte unchanged.

![Single-occurrence planets](../tmp/ex-face-planets-after/EX-LEVEL6_3-6000-32_9.bmp)

VR rear view: stars continue without repeating those planets behind the player.

![VR rear stars](../tmp/vr-face-planets-rear/live-scene-left.bmp)

## EX 5-2: tunnel wall color

Before:

![Black tunnel margins](../tmp/background-audit-later-ex-current/EX-LEVEL5_2-6000-32_9.bmp)

After: solid margins use the source wall color. Native 4:3 and the wide image's
native center are unchanged; gameplay geometry is not clipped to that center.

![Purple tunnel margins](../tmp/ex-tunnel-border-after/EX-LEVEL5_2-6000-32_9.bmp)

VR rear view is the same solid wall color, without duplicate tunnel artwork:

![VR solid tunnel surround](../tmp/vr-tunnel-wall-current/live-scene-left.bmp)

## F1 runtime options

Real SDL F1 events open, resume and reopen the menu in Original and EX.
This EX capture shows Experience locked and the game retained behind the menu.
It is not proof of every submenu action or physical-controller compatibility.

![Runtime options with locked experience](../tmp/runtime-options-current-ex/runtime-options.bmp)

## Original Black Hole: missing rear environment

The late scene retains BG_HOLE but switches to Mode 1. Before, the Mode-2-only
policy fell back to flat pink; after, the authored scrolling field surrounds
the viewer. These are actual native Vulkan eye readbacks, not headset photos.

![Before: flat pink rear](../tmp/vr-original-blackhole-current/live-scene-left.bmp)

![After: authored Black Hole surround](../tmp/vr-original-blackhole-fixed/live-scene-left.bmp)

## Planet surface continuation

Front and downward views of EX menu background 25: the source horizon keeps
its scale while a palette-matched procedural surface replaces stretched,
stacked atlas rows below it. The continuation is new rendering, not original
art hidden in the cartridge. Headset acceptance remains pending.

![Planet front](../tmp/vr-planet-center-clean-front/live-scene-left.bmp)

![Planet downward](../tmp/vr-planet-center-clean-down/live-scene-left.bmp)

## Comet: rear heated landscape

Tick-6000 rear view now has the heated sky and ground, not a flat fallback.
This static sample does not establish all Comet transitions or motion.

![Comet rear surround](../tmp/vr-comet-landscape-rear/live-scene-left.bmp)

## Hardware ray-traced pillar shadows

Fresh Original and EX checks verify ground receivers, exact CPU/GPU composition,
native shadows when DXR is unavailable, and an inactive removed Enhanced Shadows
override. Both images below were inspected. These test captures deliberately
download masks for verification; ordinary gameplay remains GPU-resident.

![Original ray-traced pillars](../tmp/current-ray-original-pillars/8b747e0f11924581ac58c9d3d41e2e66/dxr.bmp)

![EX ray-traced pillars](../tmp/current-ray-ex-pillars/a7c80ef655cd41209000457a93d0a223/dxr.bmp)

## Sector X and Meteor VR surrounds

EX Sector X rear: the previously blank view now contains stars and the planet
surface. Its small planet remains a separate forward-only patch.

![EX Sector X rear](../tmp/vr-ex-2-2-surround-fixed/live-scene-left.bmp)

Original Meteor rear: the planet-free star atlas extends behind the viewer;
the large meteor is retained separately at its authored forward location.

![Original Meteor rear](../tmp/vr-original-meteor-rear-current/live-scene-left.bmp)

## Complete regional title compositions

Original's US, Japan and Starwing compositions were inspected with ships and
foreground artwork. French/Spanish match US byte-for-byte, and German matches
English (Europe). EX remains unchanged across all six choices. The small
wrapped Original logo fragment in the far-right margin is removed.

![Original US title](../tmp/regional-title-fixed-original/language-0.bmp)

![Original Japanese title](../tmp/regional-title-fixed-original/language-1.bmp)

![English Europe Starwing title](../tmp/regional-title-fixed-original/language-5.bmp)

## Remaining scope

Colony source-state asymmetry, natural background transitions, broader feature
coverage, physical Quest/Index/Deck/Android acceptance and release signoff remain.
See GOAL-ACCEPTANCE.md and BACKGROUND-AUDIT.md for the full outstanding scope.
