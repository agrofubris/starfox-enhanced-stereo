#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>

namespace starfox::render {
// 0=OFF, 1=HALF SBS, 2=FULL SBS, 3=CROSSVIEW (full SBS, eyes swapped for
// cross-eyed free fusion), 4=INTERLACED, 5=INTERLACED-REVERSED (row-alternating
// composite for passive-polarized panels; the reversed variant swaps which eye
// owns the top output scanline), 6=HALF TAB, 7=FULL TAB (top-and-bottom /
// over-under: the left eye owns the top half, the right eye the bottom; HALF
// squeezes each eye into half the raster, FULL keeps both at full height and
// doubles it), 8=LEIA SR (Simulated Reality autostereoscopic panel). The Leia
// layout is full-SBS geometry: the weaver consumes the packed pair, and the
// same image is presented directly when the SR runtime is unavailable.
enum class StereoOutput : std::uint8_t {
    off, half_sbs, full_sbs, crossview, interlaced, interlaced_reversed,
    half_tab, full_tab, leia_sr };
// GPU-only final packing, after independent per-eye effects. Source textures
// are equally sized sampler-capable 2D textures; destination is color-target
// capable, sized by stereo_output_layout(mode,width,height). All handles share
// the caller's SDL GPU device. No active render/copy pass may be open.
// Caller owns command submission and synchronization. No CPU pixel copy.
bool enqueue_stereo_texture_pack(void* command,void* left,void* right,
    void* destination,StereoOutput mode,std::uint32_t width,std::uint32_t height);
struct StereoOutputLayout {
    std::uint32_t width{}, height{}, eye_count{};
    struct Viewport { std::uint32_t x{}, y{}, width{}, height{}; };
    std::array<Viewport, 2> eyes{};
    // The scene aspect is independent of the squeezed Half-SBS viewport.
    double scene_aspect{};
};
inline std::optional<StereoOutputLayout> stereo_output_layout(
    StereoOutput mode, std::uint32_t width, std::uint32_t height) noexcept {
    if (!width || !height) return {};
    StereoOutputLayout result{width, height, 1,
        {{{0, 0, width, height}, {0, 0, 0, 0}}}, double(width) / height};
    switch (mode) {
    case StereoOutput::off: return result;
    case StereoOutput::half_sbs:
        // Equal-sized eyes are essential to matching disparity. The caller
        // must select an even target size rather than stretch one eye.
        if (width % 2 || width < 2) return {};
        result.eyes = {{{0, 0, width / 2, height}, {width / 2, 0, width / 2, height}}};
        break;
    case StereoOutput::full_sbs:
    case StereoOutput::leia_sr:
        if (width > std::numeric_limits<std::uint32_t>::max() / 2) return {};
        result.width = width * 2;
        result.eyes = {{{0, 0, width, height}, {width, 0, width, height}}};
        break;
    case StereoOutput::crossview:
        // Identical geometry to full SBS; the packer/presenter swaps which
        // eye texture lands in each half.
        if (width > std::numeric_limits<std::uint32_t>::max() / 2) return {};
        result.width = width * 2;
        result.eyes = {{{0, 0, width, height}, {width, 0, width, height}}};
        break;
    case StereoOutput::interlaced:
    case StereoOutput::interlaced_reversed:
        // Mono-sized target; both eyes span the full raster and alternate by
        // output row. The variants differ only in the top-row eye, decided by
        // the packer/presenter.
        result.eyes = {{{0, 0, width, height}, {0, 0, width, height}}};
        break;
    case StereoOutput::half_tab:
        // Top-and-bottom: left eye on top, right eye below, each squeezed
        // vertically into half the mono raster.
        if (height % 2 || height < 2) return {};
        result.eyes = {{{0, 0, width, height / 2}, {0, height / 2, width, height / 2}}};
        break;
    case StereoOutput::full_tab:
        // Full-height over-under: the target is twice as tall and neither eye
        // is squeezed.
        if (height > std::numeric_limits<std::uint32_t>::max() / 2) return {};
        result.height = height * 2;
        result.eyes = {{{0, 0, width, height}, {0, height, width, height}}};
        break;
    default: return {};
    }
    result.eye_count = 2;
    return result;
}

// Parallel (not toe-in) cameras: translate world X by -eye_x before
// projection, then add projection_offset_x in normalized device space.
// This keeps vertical disparity zero and the convergence plane stationary.
struct StereoEyeProjection { double eye_x{}, projection_offset_x{}; };
inline std::optional<std::array<StereoEyeProjection, 2>> stereo_eye_projections(
    double separation, double convergence, double horizontal_focal_scale) noexcept {
    if (!std::isfinite(separation) || separation <= 0
        || !std::isfinite(convergence) || convergence <= 0
        || !std::isfinite(horizontal_focal_scale) || horizontal_focal_scale <= 0)
        return {};
    const double eye = separation / 2;
    const double offset = horizontal_focal_scale * (eye / convergence);
    if (!std::isfinite(offset) || offset == 0) return {};
    return std::array<StereoEyeProjection, 2>{{{-eye, -offset}, {eye, offset}}};
}
// The source projection's default focal length in source pixels (MOBJ.MC's
// 256). Planar layers have no model focal, so this is their reference scale.
inline constexpr double stereo_source_focal{256.0};
// Screen-space parallax for a flat layer that represents an infinitely distant
// plane, in source pixels. The left eye shifts the layer left and the right eye
// shifts it right, which places a 2D backdrop behind the model world instead of
// on the convergence plane. Positive values move the layer right on screen.
inline std::int32_t stereo_planar_parallax_px(
    unsigned eye, double separation, double convergence) noexcept {
    if (eye > 1U || !std::isfinite(separation) || separation <= 0
        || !std::isfinite(convergence) || convergence <= 0) return 0;
    const auto magnitude = static_cast<std::int32_t>(std::lround(
        stereo_source_focal * (separation * .5) / convergence));
    return eye ? magnitude : -magnitude;
}
}
