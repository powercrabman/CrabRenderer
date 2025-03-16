#pragma once

#include "D11Utils.h"

namespace crab
{

class Texture2D;
class RenderTarget;
class DepthBuffer;

class Swapchain
{
public:
    void Init(const SwapChainSetting& in_setting, const Int2& in_screenSize, HWND in_hWnd);

    void Present() const;
    void OnResize(Int2 in_size);

    void EnableVSynchronize(bool in_enable) { m_vsync = in_enable; }

    bool IsVSync() const { return m_vsync; }
    bool UseMSAA() const { return m_enableMSAA; }
    bool IsHDR() const { return m_enableHDRRendering; }

    Ref<DepthBuffer> GetDepthBuffer() const;

    Ref<RenderTarget> GetBackBuffer() const;
    Ref<RenderTarget> GetBackBufferHDR() const;

private:
    void _CreateResources(const Int2& in_size);
    void _CreateHDRRenderTarget(const Int2& in_size);
    void _InitMSAA();

    ComPtr<IDXGISwapChain1> m_swapChain;

    Ref<RenderTarget> m_backBuffer;

    // Main backBuffer
    eFormat m_backBufferFormat = {};

    // HDR backBuffer
    Ref<RenderTarget> m_backBufferHDR;   // MSAA
    bool              m_enableHDRRendering = false;

    // Depth buffer
    Ref<DepthBuffer> m_depthBuffer;   // back buffer dsv
    eFormat          m_depthBufferFormat = {};

    // MSAA
    bool   m_enableMSAA      = false;
    uint32 m_MSAASampleCount = 1;
    uint32 m_MSAAQuality     = 0;

    // vsync
    bool m_vsync = false;
};

}   // namespace crab