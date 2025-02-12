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
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc  = {};
    swapChainDesc.Width                  = width;
    swapChainDesc.Height                 = height;
    swapChainDesc.Format                 = eD11_FormatToDXGIFormat(in_setting.swapChainFormat);
    swapChainDesc.Stereo                 = FALSE;
    swapChainDesc.SampleDesc.Count       = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality     = 0;   // Not use MSAA
    swapChainDesc.BufferUsage            = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.BufferCount            = 2;
    swapChainDesc.Scaling                = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect             = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode              = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags                  = 0;
    swapchain->m_vsync                   = in_setting.enableVSync;
    swapchain->m_backbufferFormat        = in_setting.swapChainFormat;
    swapchain->m_depthBufferFormat       = in_setting.depthBufferFormat;
    swapchain->m_enableDepthBuffer       = in_setting.enableDepthBuffer;
    swapchain->m_enableFloatRenderTarget = in_setting.enableFloatRenderTarget;
    swapchain->m_enableMSAA              = in_setting.enableMSAA;
    swapchain->m_MSAADesc.quality        = in_setting.MSAAQuality;
    swapchain->m_MSAADesc.sampleCount    = in_setting.MSAASampleCount;
    swapchain->m_MSAADesc.format         = in_setting.enableMSAA ? eD11_Format::Float16x4 : eD11_Format::Unknown;

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
    m_backBufferRTV.reset();
    m_floatBackBufferRTV.reset();
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

void D11_Swapchain::SetMSAA(bool in_enable, int32 in_sampleCount, int32 in_quality)
{
    auto [width, height] = GetAppWindow().GetWindowSize();

    if (in_enable)
    {
        uint32 bestQualityCount;
        if (SUCCEEDED(D11_API->GetDevice()->CheckMultisampleQualityLevels(
                eD11_FormatToDXGIFormat(eD11_Format::Float16x4),
                in_sampleCount,
                &bestQualityCount)))
        {
            if (in_quality <= MSAA_MAX_QUALITY)
            {
                in_quality = static_cast<int>(bestQualityCount) - 1;
            }
            else
            {
                in_quality = std::min(static_cast<int>(bestQualityCount - 1), in_quality);
            }

            m_MSAADesc.sampleCount = in_sampleCount;
            m_MSAADesc.quality     = in_quality;
            m_MSAADesc.format      = eD11_Format::Float16x4;
        }
        else
        {
            m_MSAADesc.format      = eD11_Format::Unknown;
            m_MSAADesc.sampleCount = 1;
            m_MSAADesc.quality     = 0;
        }
    }
    else
    {
        m_MSAADesc.format      = eD11_Format::Unknown;
        m_MSAADesc.sampleCount = 1;
        m_MSAADesc.quality     = 0;
    }

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

Ref<D11_RenderTarget> D11_Swapchain::GetBackBufferRT() const
{
    return m_backBufferRTV;
}

Ref<D11_Texture2D> D11_Swapchain::GetBackBufferTex() const
{
    return m_backBufferRTV->GetTexture();
}

Ref<D11_RenderTarget> D11_Swapchain::GetFloatBackBufferRT() const
{
    if (m_enableFloatRenderTarget)
    {
        return m_floatBackBufferRTV;
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<D11_Texture2D> D11_Swapchain::GetFloatBackBufferTex() const
{
    if (m_enableFloatRenderTarget)
    {
        return m_floatBackBufferRTV->GetTexture();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<D11_Texture2D> D11_Swapchain::GetFloatBackBufferResolveTex() const
{
    if (m_enableFloatRenderTarget)
    {
        return m_floatBackBufferResolveRTV->GetTexture();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

void D11_Swapchain::ResolveFloatBackBufferTex() const
{
    if (m_enableFloatRenderTarget)
    {
        D11_API->GetContext()->ResolveSubresource(
            m_floatBackBufferResolveTex.Get(),
            0,
            m_floatBackBufferTex.Get(),
            0,
            eD11_FormatToDXGIFormat(m_floatBackBufferResolveRTV->GetFormat()));
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
    vp.width    = static_cast<float>(in_width);
    vp.height   = static_cast<float>(in_height);
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

    m_backBufferRTV = D11_RenderTarget::Create(backBufferTexture.Get());

    // depth stencil
    if (m_enableDepthBuffer)
    {
        m_depthBuffer = D11_DepthBuffer::Create(
            in_width,
            in_height,
            m_depthBufferFormat,
            m_MSAADesc.sampleCount,
            m_MSAADesc.quality);
    }
}

void D11_Swapchain::_CreateFloatRenderTarget(uint32 in_width, uint32 in_height)
{
    auto d = D11_API->GetDevice();

    m_floatBackBufferTex = ID3D11Texture2DUtil::CreateTexture2D(
        in_width,
        in_height,
        eD11_Format::Float16x4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0,
        m_MSAADesc.sampleCount,
        m_MSAADesc.quality);

    m_floatBackBufferRTV = D11_RenderTarget::Create(m_floatBackBufferTex.Get());

    m_floatBackBufferResolveTex = ID3D11Texture2DUtil::CreateTexture2D(
        in_width,
        in_height,
        eD11_Format::Float16x4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0,
        1,
        0);

    m_floatBackBufferResolveRTV = D11_RenderTarget::Create(m_floatBackBufferResolveTex.Get());
}

}   // namespace crab