#pragma once

#include "D11_Utiles.h"

namespace crab
{

class D11_Texture2D;
class D11_RenderTarget;
class D11_DepthBuffer;

class D11_Swapchain
{
public:
    static Ref<D11_Swapchain> Create(const SwapChainSetting& in_setting);

    void Present();
    void OnResize(uint32 in_width, uint32 in_height);

    void SetVSync(bool in_vsync) { m_vsync = in_vsync; }
    void SetMSAA(bool in_enable, uint32 in_sampleCount, uint32 in_quality);

    bool IsVSync() const { return m_vsync; }
    bool IsMSAA() const { return m_enableMSAA; }
    bool HasFloatRenderTarget() const { return m_useFloatRenderTarget; }
    bool HasDepthBuffer() const { return m_useDepthBuffer; }

    Viewport GetViewport() const;

    Ref<D11_DepthBuffer> GetDepthBuffer() const;

    Ref<D11_RenderTarget> GetBackBuffer() const;
    Ref<D11_Texture2D>    GetBackBufferTexture() const;

    Ref<D11_RenderTarget> GetFloatRenderTarget() const;
    Ref<D11_Texture2D>    GetFloatBackBufferTexture() const;
    Ref<D11_Texture2D>    GetFloatBackBufferResolveTexture() const;
    void                  ResolveFloatBackBuffer();

private:
    void _CreateResources(uint32 in_width, uint32 in_height);
    void _CreateFloatRenderTarget(uint32 in_width, uint32 in_height);

    ComPtr<IDXGISwapChain1> m_swapChain;

    Ref<D11_RenderTarget> m_backBuffer;
    Ref<D11_DepthBuffer>  m_depthBuffer;
    Viewport              m_viewport;

    // for support float render target (HDR)
    Ref<D11_RenderTarget>   m_floatBackBuffer;
    ComPtr<ID3D11Texture2D> m_floatBackBufferTexture;

    Ref<D11_RenderTarget>   m_floatBackBufferResolve;
    ComPtr<ID3D11Texture2D> m_floatBackBufferResolveTexture;

    DXGI_FORMAT m_backbufferFormat;
    DXGI_FORMAT m_depthStencilFormat;
    bool        m_useDepthBuffer;
    bool        m_useFloatRenderTarget;
    bool        m_vsync;

    bool     m_enableMSAA;
    MSAADesc m_MSAADesc;
};

}   // namespace crab