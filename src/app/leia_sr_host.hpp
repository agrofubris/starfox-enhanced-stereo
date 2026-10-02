#pragma once
#include "starfox/render/sdl_d3d12_bridge.h"
#include <cstdint>
#include <iostream>

#if defined(STARFOX_LEIASR)
#include <windows.h>
#include <d3d12.h>
#include "SR.hpp"

// Leia SR (Simulated Reality) autostereoscopic output. Thin host around the
// SR-lib DX12 weaver: created lazily on the present thread, fed the packed
// side-by-side texture every frame, and dropped before the device/swapchain.
// The SR runtime, service and display all live on the user's machine; a
// missing one just leaves the host unavailable so the caller presents the
// same side-by-side image directly.
class LeiaSrHost {
public:
    LeiaSrHost() = default;
    ~LeiaSrHost() { release(); }
    LeiaSrHost(const LeiaSrHost&) = delete;
    LeiaSrHost& operator=(const LeiaSrHost&) = delete;

    // d3d12_device: ID3D12Device*, hwnd: HWND. False when the SR runtime, the
    // SR service or an SR display is missing; this host then stays unavailable
    // (no retry) until release() re-arms it for a new device.
    bool ensure(void* d3d12_device, void* hwnd) {
        if (sr_) return true;
        if (unavailable_ || !d3d12_device || !hwnd) { unavailable_ = true; return false; }
        SimulatedReality::SRInterfaceDX12* sr = nullptr;
        const HRESULT hr = SimulatedReality::CreateSRInterfaceDX12(
            static_cast<ID3D12Device*>(d3d12_device), static_cast<HWND>(hwnd), &sr);
        if (FAILED(hr) || !sr) {
            unavailable_ = true;
            std::cerr << "leia-sr: CreateSRInterfaceDX12 failed (hr=0x" << std::hex
                << static_cast<unsigned long>(hr) << std::dec
                << "); presenting side-by-side\n";
            return false;
        }
        sr_ = sr;
        // The packed intermediate and the SDR swap chain are both UNORM and the
        // compose already encoded the game's gamma, so the weaver must not
        // convert in either direction (that would double-gamma the panel).
        sr_->SetShaderSRGBConversion(false, false);
        std::cerr << "leia-sr: weaver initialized\n";
        return true;
    }
    bool available() const noexcept { return sr_ != nullptr; }
    bool unavailable() const noexcept { return unavailable_; }
    // command_list: ID3D12GraphicsCommandList* with the swap-chain backbuffer
    // bound; native_source: the complete 2W x H side-by-side resource.
    bool weave(void* command_list, void* native_source, std::uint32_t width,
        std::uint32_t height, std::uint32_t output_format) {
        if (!sr_ || !command_list || !native_source || !width || !height) return false;
        try {
            // Rebind every frame: sizes and formats come off the resource desc,
            // and a cached view can go stale across a resize.
            sr_->SetInputTexture(static_cast<ID3D12Resource*>(native_source));
            if (output_format != output_format_) {
                sr_->SetOutputFormat(static_cast<DXGI_FORMAT>(output_format));
                output_format_ = output_format;
            }
            const D3D12_VIEWPORT viewport{0.0f, 0.0f, static_cast<float>(width),
                static_cast<float>(height), 0.0f, 1.0f};
            const D3D12_RECT scissor{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
            sr_->Weave(static_cast<ID3D12GraphicsCommandList*>(command_list), viewport, scissor);
            return true;
        } catch (const std::exception& error) {
            std::cerr << "leia-sr: weave threw: " << error.what() << '\n';
            return false;
        } catch (...) {
            std::cerr << "leia-sr: weave threw\n";
            return false;
        }
    }
    // Delete() destroys the weaver and then the context, in that order; the
    // interface itself is never `delete`d. Call on the present thread before
    // releasing the device or any texture the weaver sampled.
    void release() noexcept {
        if (sr_) {
            sr_->Delete();
            sr_ = nullptr;
        }
        output_format_ = 0U;
        unavailable_ = false;
    }
    // A failed weave stays failed for this device: keep presenting the plain
    // side-by-side image instead of retrying a broken runtime every frame.
    void disable() noexcept {
        if (sr_) {
            sr_->Delete();
            sr_ = nullptr;
        }
        output_format_ = 0U;
        unavailable_ = true;
    }
private:
    SimulatedReality::SRInterfaceDX12* sr_{};
    std::uint32_t output_format_{};
    bool unavailable_{};
};
#else
class LeiaSrHost {
public:
    bool ensure(void*, void*) { return false; }
    bool available() const noexcept { return false; }
    bool unavailable() const noexcept { return true; }
    bool weave(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t) { return false; }
    void release() noexcept {}
    void disable() noexcept {}
};
#endif
