#include "CrabPch.h"

#include "D11_Swapchain.h"

#include "AppWindow.h"
#include "D11_DepthBuffer.h"
#include "D11_RenderTarget.h"
#include "D11_Renderer.h"
#include "D11_Texture.h"

namespace crab
{

Ref<D11_Swapchain> D11_Swapchain::Create(const SwapChainSetting& in_setting)
{
    auto               d         = D11_API->GetDevice();
    Ref<D11_Swapchain> swapchain = CreateRef<D11_Swapchain>();
    AppWindow&         window    = GetApplication().GetAppWindow();

    auto [width, height] = window.GetWindowSize();

    // - Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width                 = width;
    swapChainDesc.Height                = height;
    swapChainDesc.Format                = in_setting.swapChainFormat;
    swapChainDesc.Stereo                = FALSE;
    swapChainDesc.SampleDesc.Count      = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality    = 0;   // Not use MSAA
    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.BufferCount           = 2;
    swapChainDesc.Scaling               = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags                 = 0;
    swapchain->m_vsync                  = in_setting.useVSync;
    swapchain->m_backbufferFormat       = in_setting.swapChainFormat;
    swapchain->m_depthStencilFormat     = in_setting.depthStencilFormat;
    swapchain->m_useDepthBuffer         = in_setting.useDepthStencil;
    swapchain->m_useFloatRenderTarget   = in_setting.useFloatRenderTarget;
    swapchain->m_enableMSAA             = in_setting.enableMSAA;
    swapchain->m_MSAADesc.quality       = in_setting.MSAAQuality;
    swapchain->m_MSAADesc.sampleCount   = in_setting.MSAASampleCount;
    swapchain->m_MSAADesc.format        = in_setting.enableMSAA ? DXGI_FORMAT_R16G16B16A16_FLOAT : DXGI_FORMAT_UNKNOWN;

    ComPtr<IDXGIDevice2> dxgiDevice;
    D11_ASSERT(d->QueryInterface(__uuidof(IDXGIDevice2), (void**)dxgiDevice.GetAddressOf()), "QueryInterface Fail.");

    ComPtr<IDXGIAdapter> dxgiAdapter;
    D11_ASSERT(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.GetAddressOf()), "GetParent Fail.");

    ComPtr<IDXGIFactory2> dxgiFactory;
    D11_ASSERT(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.GetAddressOf()), "GetParent Fail.");

    D11_ASSERT(dxgiFactory->CreateSwapChainForHwnd(
                   d.Get(),
                   (HWND)window.GetNativeWindowHandle(),
                   &swapChainDesc,
                   nullptr,
                   nullptr,
                   swapchain->m_swapChain.GetAddressOf()),
               "CreateSwapChainForHwnd Fail.");

    swapchain->_CreateResources(width, height);

    return swapchain;
}

void D11_Swapchain::Present()
{
    static DXGI_PRESENT_PARAMETERS presentParameters = {};
    presentParameters.DirtyRectsCount                = 0;
    presentParameters.pDirtyRects                    = nullptr;
    presentParameters.pScrollRect                    = nullptr;
    presentParameters.pScrollOffset                  = nullptr;

    m_swapChain->Present1(m_vsync ? 1 : 0, 0, &presentParameters);
}

void D11_Swapchain::OnResize(uint32 in_width, uint32 in_height)
{
    m_backBuffer.reset();
    m_floatBackBuffer.reset();
    m_depthBuffer.reset();

    D11_ASSERT(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers Fail.");
    Viewport vp = {};
    vp.width    = (float)in_width;
    vp.height   = (float)in_height;
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = 0.f;
    vp.y        = 0.f;

    _CreateResources(in_width, in_height);
}

void D11_Swapchain::SetMSAA(bool in_enable, uint32 in_sampleCount, uint32 in_quality)
{
    m_enableMSAA           = in_enable;
    m_MSAADesc.sampleCount = in_sampleCount;
    m_MSAADesc.quality     = in_quality;

    auto [width, height] = GetAppWindow().GetWindowSize();
    _CreateFloatRenderTarget(width, height);
}

Viewport D11_Swapchain::GetViewport() const
{
    return m_viewport;
}

Ref<D11_DepthBuffer> D11_Swapchain::GetDepthBuffer() const
{
    return m_depthBuffer;
}

Ref<D11_RenderTarget> D11_Swapchain::GetBackBuffer() const
{
    return m_backBuffer;
}

Ref<D11_Texture2D> D11_Swapchain::GetBackBufferTexture() const
{
    return m_backBuffer->GetTexture();
}

Ref<D11_RenderTarget> D11_Swapchain::GetFloatRenderTarget() const
{
    if (m_useFloatRenderTarget)
    {
        return m_floatBackBuffer;
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<D11_Texture2D> D11_Swapchain::GetFloatBackBufferTexture() const
{
    if (m_useFloatRenderTarget)
    {
        return m_floatBackBuffer->GetTexture();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<D11_Texture2D> D11_Swapchain::GetFloatBackBufferResolveTexture() const
{
    if (m_useFloatRenderTarget)
    {
        return m_floatBackBufferResolve->GetTexture();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

void D11_Swapchain::ResolveFloatBackBuffer()
{
    if (m_useFloatRenderTarget)
    {
        D11_API->GetContext()->ResolveSubresource(
            m_floatBackBufferResolveTexture.Get(),
            0,
            m_floatBackBufferTexture.Get(),
            0,
            m_floatBackBufferResolve->GetFormat());
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
    }
}

void D11_Swapchain::_CreateResources(uint32 in_width, uint32 in_height)
{
    auto d = D11_API->GetDevice();

    // viewport
    Viewport vp = {};
    vp.width    = (float)in_width;
    vp.height   = (float)in_height;
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = 0.f;
    vp.y        = 0.f;
    m_viewport  = vp;

    _CreateFloatRenderTarget(in_width, in_height);

    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr;
    D11_ASSERT(m_swapChain->GetBuffer(0,
                                      __uuidof(ID3D11Texture2D),
                                      (void**)backBufferTexture.GetAddressOf()),
               "GetBuffer Fail.");

    m_backBuffer = D11_RenderTarget::Create(backBufferTexture.Get());

    // depth stencil
    if (m_useDepthBuffer)
    {
        DepthBufferSetting setting = {};
        setting.format             = m_depthStencilFormat;
        setting.msaaDesc           = m_MSAADesc;
        setting.width              = in_width;
        setting.height             = in_height;

        m_depthBuffer = D11_DepthBuffer::Create(setting);
    }
}

void D11_Swapchain::_CreateFloatRenderTarget(uint32 in_width, uint32 in_height)
{
    auto d = D11_API->GetDevice();

    if (!m_useFloatRenderTarget)
    {
        m_MSAADesc = { DXGI_FORMAT_UNKNOWN, 1, 0 };
        return;
    }

    uint32 maxQuality = 0;
    if (m_enableMSAA && SUCCEEDED(d->CheckMultisampleQualityLevels(
                            m_MSAADesc.format,
                            m_MSAADesc.sampleCount,
                            &maxQuality)))
    {
        maxQuality = maxQuality - 1;

        if (m_MSAADesc.quality <= -1)
            m_MSAADesc.quality = maxQuality;

        m_MSAADesc.quality = std::min(m_MSAADesc.quality, (int32)maxQuality);
        m_MSAADesc.format  = DXGI_FORMAT_R16G16B16A16_FLOAT;
    }
    else
    {
        m_enableMSAA = false;
        m_MSAADesc   = { DXGI_FORMAT_UNKNOWN, 1, 0 };
    }

    D3D11_TEXTURE2D_DESC desc {
        .Width          = in_width,
        .Height         = in_height,
        .MipLevels      = 1,
        .ArraySize      = 1,
        .Format         = DXGI_FORMAT_R16G16B16A16_FLOAT,
        .SampleDesc     = { (uint32)m_MSAADesc.sampleCount, (uint32)m_MSAADesc.quality },
        .Usage          = D3D11_USAGE_DEFAULT,
        .BindFlags      = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        .CPUAccessFlags = 0,
        .MiscFlags      = 0
    };

    m_floatBackBufferTexture = ID3D11Texture2DFactory::CreateID3D11Texture2D(desc);
    m_floatBackBuffer        = D11_RenderTarget::Create(m_floatBackBufferTexture.Get());

    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;

    m_floatBackBufferResolveTexture = ID3D11Texture2DFactory::CreateID3D11Texture2D(desc);
    m_floatBackBufferResolve        = D11_RenderTarget::Create(m_floatBackBufferResolveTexture.Get());

    if (!m_enableMSAA)
        m_MSAADesc = { DXGI_FORMAT_UNKNOWN, 1, 0 };
}

}   // namespace crab