#pragma once

#include "D11Utils.h"

namespace crab
{

class Image2D;
class RenderTarget;
class DepthBuffer;

class Swapchain
{
public:
    static Ref<Swapchain> Create(const SwapChainSetting& in_setting, const Int2& in_screenSize, HWND in_hWnd);

    void Present() const;
    void OnResize(Int2 in_size);

    void EnableVSynchronize(bool in_enable) { m_vsync = in_enable; }
    void EnableMSAA(bool in_enable);

    bool IsVSync() const { return m_vsync; }
    bool IsMSAA() const { return m_enableMSAA; }
    bool IsHDR() const { return m_enableHDRRendering; }

    Viewport GetViewport() const;

    Ref<DepthBuffer> GetDepthBuffer() const;
    Ref<DepthBuffer> GetDepthOnlyBuffer() const;
    Ref<Image2D>     GetDepthOnlyBufferImage() const;

    Ref<RenderTarget> GetBackBuffer() const;
    Ref<Image2D>      GetBackBufferImage() const;

    Ref<RenderTarget> GetBackBufferHDR() const;
    Ref<Image2D>      GetBackBufferHDRImage() const;
    Ref<Image2D>      GetResolvedBackBufferImage() const;
    void              ResolveBackBuffer() const;

private:
    void _CreateResources(const Int2& in_size);
    void _CreateHDRRenderTarget(const Int2& in_size);

    ComPtr<IDXGISwapChain1> m_swapChain;

    Ref<RenderTarget> m_backBuffer;   // No MSAA
    Viewport          m_viewport;

    // Main backBuffer
    eFormat m_backBufferFormat = {};

    // HDR backBuffer
    Ref<RenderTarget>       m_backBufferHDR;   // MSAA
    ComPtr<ID3D11Texture2D> m_backBufferHDRImage;
    Ref<RenderTarget>       m_resolvedBackBuffer;
    ComPtr<ID3D11Texture2D> m_resolvedBackBufferImage;
    bool                    m_enableHDRRendering = false;

    // Depth buffer
    Ref<DepthBuffer> m_depthBuffer;   // back buffer dsv
    Ref<DepthBuffer> m_depthOnlyBuffer;
    eFormat          m_depthBufferFormat = {};

    // MSAA
    bool   m_enableMSAA      = false;
    uint32 m_MSAASampleCount = 1;
    uint32 m_MSAAQuality     = 0;

    // vsync
    bool m_vsync = false;
};

}   // namespace crab