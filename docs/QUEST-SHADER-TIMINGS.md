# Quest shader compilation measurements

Measured 2026-09-11 on the authorized Quest 3, Adreno 740,
driver 2150912009. `starfox_vr_shader_bench` builds production compute
pipelines twice in a standalone Vulkan process. It does not dispatch work,
launch VR, or read/change saves. Target is excluded from normal builds.

| Mode | Clipping first / second (ms) | Projection first / second (ms) |
| --- | --- | --- |
| Default, no cache | 18391 / 18901 | 3217 / 3060 |
| Disable optimization | 18280 / 18730 | 3066 / 3263 |
| Explicit pipeline cache | 18243 / 5.52 | 3000 / 2.31 |

All eight compute stages were created successfully in both passes of each run.
The cache is reused after destroying each first-pass pipeline; these are actual
pipeline creations, not retained handles. Default optimization remains enabled
in production. `--fast-compile` and `--cache` select diagnostic alternatives.
VulkanSpanPipeline now accepts an optional caller-owned cache, default null.

This establishes an expensive first-use compilation problem and the value of
explicit cache reuse. It does not establish gameplay FPS, dispatch performance,
or persistent cache behavior across process launches. The previously installed
APK still uses retained pipelines and lazy compilation, not this explicit cache.

## Persistent compute cache integration

VR startup now creates a caller-owned cache and passes it to live compute
pipeline creation. Quest stores it beside cartridge saves in `shader-cache`,
without changing save files. Files are scoped by vendor/device/driver, checked
against the Vulkan cache UUID/header and a payload checksum, and capped at
64 MiB. Failed cache creation retries without initial data. Read/write failure
does not prevent rendering. Checkpoints occur only after new compute pipelines,
using temporary-file rename (atomic replacement on Android), not per frame.

A separate-process Quest test of this production cache class gave 18086 ms
clipping and 3004 ms projection cold, then **5.01 ms / 2.38 ms** after restarting
the diagnostic. All eight stages succeeded. All 13 host VR tests pass (11.36 s).

Cache regression tests now cover persisted reload, repeated Windows replacement,
UUID mismatch, checksum corruption, truncated files, driver rejection followed
by empty-cache retry, total creation failure, incomplete data retrieval and
unwritable paths. Windows uses replace-existing MoveFileExW; Android retains
rename replacement. Host cache tests and rebuilt VR targets pass.

Graphics pipeline creation now shares the same cache for live models, legacy
ordered packets, backgrounds, sprites, menu, bomb circles and shutters. The
cache is borrowed only during creation, with checkpoints after new pipelines;
retained draw calls do not access disk. The standalone test triangle retains
its uncached diagnostic path. Host Vulkan scene/compute/stereo readback
regressions pass in `tmp/vr-graphics-cache-regression`; that diagnostic still
uses default null cache and is not proof of a cached graphics speedup.

The host scene diagnostic now accepts `capture-directory --cache`, routing its
synthetic triangle, line, textured, depth and circle-blending pipelines through
the persistent cache. Two separate-process runs pass all readback assertions;
all **100 BMP captures** from the warm cached run exactly match the previous
uncached regression by SHA-256. This verifies the sampled Intel GPU output,
not all Quest scenes or a graphics compilation speedup. Artifacts:
`tmp/vr-graphics-cache-cached` and `tmp/vr-graphics-cache-regression`.

Remaining: Quest cached graphics timing/output comparisons and physical cold/warm app
launch measurements. Desktop currently has no configured cache path.
First-ever compilation remains
expensive; cache reuse cannot eliminate that initial work.
Preserve exact shader arithmetic and rendered output; do not trade away parity
to hide compilation stalls. Steady-state CPU/GPU timings still require work.

## Physical Adreno dispatch/readback verification

Built the standalone scene diagnostic for Android (explicit target, excluded
from normal APK builds), then ran it on Quest 3 / Adreno 740 without an XR
session. The run completed successfully: exact projection/visibility/BSP/clip/
span chain, ordered warp PRNG/material expansion, lazy pipeline transitions,
keyed upload reuse, stereo/depth, circle blending, decals and shutter checks.
All 100 pulled BMPs in `tmp/quest-adreno-render-proof` exactly match the Intel
baseline by SHA-256. This is GPU-generated proof on the physical headset's
hardware, not in-headset optical capture or a full gameplay acceptance test.

The synthetic combined-scene CPU setup benchmark reported retained update
10.1614 microseconds versus uncached rebuild 4.10734 seconds. Rebuild includes
shader creation; do not interpret this ratio as a gameplay FPS improvement.
Several independent compute fixtures intentionally still compile without a
cache, explaining much of the diagnostic's long runtime. Its graphics cache
path was enabled; a full warm-process Quest comparison remains outstanding.

## Unchanged model compute reuse

Live source-scene initialization now acknowledges completed model compute work
after the caller's two-eye fence boundary. Identical input snapshots retain
projection/visibility/BSP/clip/span output, so recording can omit those compute
dispatches while still drawing both eyes with current headset transforms.
Recording alone never marks outputs reusable. Input/palette writes invalidate
reuse; new allocations start dirty. Existing direct diagnostic callers continue
dispatching unless they explicitly acknowledge completed GPU work.

Host tests cover completion versus recording, unchanged input retention and
pose/palette invalidation. Vulkan rendering checks pass and all 100 captures in
`tmp/vr-compute-reuse-proof` match the baseline exactly. All 14 VR tests pass.
No quantified Quest FPS gain is claimed; moving/interpolated poses still require
compute, and the prior Adreno run predates this optimization.

Live profiling now reports completed-eye count plus average/maximum elapsed
submission-to-fence-observation milliseconds, alongside CPU logic/model/upload/
layer times. These include command recording and polling/scheduling delays;
they are not GPU timestamp measurements. Counters increment only on completion,
not repeated pending polls, and reset when collected. Mock fence tests cover
pending/retry/completion accounting. New headset traces are still required.

Projection shader cleanup: select exact versus compensated camera operands
before software-double multiply/divide, instead of computing a fallback result
and overwriting it. Both paths retain their original chosen operands and
rounding. Regenerated SPIR-V/Metal with strict IEEE settings. Exact host GPU
projection checks pass and all 100 captures in `tmp/vr-projection-single-divide`
match baseline. Quest compiled the new projection shader in 2867 ms cold and
2.03 ms cached; this is not a measured dispatch/FPS improvement. Physical
Adreno dispatch verification of this new shader remains required.

Further projection cleanup makes exact and compensated screen projection
mutually exclusive. Previously the compensated multiply/divide was executed
before its result was replaced by exact projection. Selected arithmetic and
rounding remain unchanged. Strict IEEE SPIR-V/Metal generation checks pass;
host exact projection/readback checks pass; all 100 BMPs in
`tmp/vr-projection-exclusive-paths` match the baseline. Host builds pass.
This follow-up is local, not yet in the installed Quest APK; combined Adreno
dispatch and performance verification of both projection cleanups remains.

Combined Adreno refresh completed after both projection cleanups and compute
reuse changes: full standalone dispatch/readback suite passes on Quest 3,
including the completion/invalidation assertions. All 100 pulled BMPs in
`tmp/quest-adreno-projection-refresh` match the unchanged Intel baseline by
SHA-256. This supersedes the pending physical dispatch checks above, but does
not establish sustained gameplay performance. Retained scene CPU setup measured
10.04 us; cold rebuild 3.84 s includes shader creation and is not frame FPS.
