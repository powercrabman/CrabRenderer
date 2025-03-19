#pragma once

#include "RenderUtils.h"

namespace crab
{

class Texture2D;
class RenderTarget;
class DepthBuffer;

class SwapChain
{
public:
    void Init(const RendererSetting& in_setting, const Int2& in_screenSize, MSAA in_msaa, HDR in_hdr, HWND in_hWnd);

    void Present(bool in_vsync) const;
    void OnResize(Int2 in_size);

    Ref<DepthBuffer> GetDepthBuffer() const;

    Ref<RenderTarget> GetBackBuffer() const;
    Ref<RenderTarget> GetBackBufferHDR() const;

private:
    void _CreateResources(const Int2& in_size);
    void _CreateHDRRenderTarget(const Int2& in_size);

    ComPtr<IDXGISwapChain1> m_swapChain;

    // Main backBuffer
    Ref<RenderTarget> m_backBuffer;
    eFormat           m_backBufferFormat = {};

    // Main Depth buffer
    Ref<DepthBuffer> m_depthBuffer;
    eFormat          m_depthBufferFormat = {};

    // HDR backBuffer
    Ref<RenderTarget> m_backBufferHDR;
};

}   // namespace crab