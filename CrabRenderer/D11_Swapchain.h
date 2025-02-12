#pragma once

#include "D11_Utils.h"

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
    void SetMSAA(bool in_enable, int32 in_sampleCount, int32 in_quality);

    bool IsVSync() const { return m_vsync; }
    bool IsMSAA() const { return m_enableMSAA; }
    bool HasFloatRenderTarget() const { return m_enableFloatRenderTarget; }
    bool HasDepthBuffer() const { return m_enableDepthBuffer; }

    Viewport GetViewport() const;

    Ref<D11_DepthBuffer> GetDepthBuffer() const;

    Ref<D11_RenderTarget> GetBackBufferRT() const;
    Ref<D11_Texture2D>    GetBackBufferTex() const;

    Ref<D11_RenderTarget> GetFloatBackBufferRT() const;
    Ref<D11_Texture2D>    GetFloatBackBufferTex() const;
    Ref<D11_Texture2D>    GetFloatBackBufferResolveTex() const;
    void                  ResolveFloatBackBufferTex() const;

private:
    void _CreateResources(uint32 in_width, uint32 in_height);
    void _CreateFloatRenderTarget(uint32 in_width, uint32 in_height);

    ComPtr<IDXGISwapChain1> m_swapChain;

    Ref<D11_RenderTarget> m_backBufferRTV;
    Ref<D11_DepthBuffer>  m_depthBuffer;
    Viewport              m_viewport;

    // for support float render target (HDR)
    Ref<D11_RenderTarget>   m_floatBackBufferRTV;
    ComPtr<ID3D11Texture2D> m_floatBackBufferTex;

    Ref<D11_RenderTarget>   m_floatBackBufferResolveRTV;
    ComPtr<ID3D11Texture2D> m_floatBackBufferResolveTex;

    eD11_Format      m_backbufferFormat;
    bool             m_enableFloatRenderTarget;

    eD11_DepthFormat m_depthBufferFormat;
    bool             m_enableDepthBuffer;

    bool     m_enableMSAA;
    MSAADesc m_MSAADesc;

    bool             m_vsync;
};

}   // namespace crab