#include "CrabPch.h"

#include "Swapchain.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Textures.h"

namespace crab
{

Ref<Swapchain> Swapchain::Create(
    const SwapChainSetting& in_setting,
    const Int2&             in_screenSize,
    HWND                    in_hWnd)
{
    auto           d         = GetRenderer().GetDevice();
    Ref<Swapchain> swapChain = CreateRef<Swapchain>();

    // - Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width                 = in_screenSize.x;
    swapChainDesc.Height                = in_screenSize.y;
    swapChainDesc.Format                = static_cast<DXGI_FORMAT>(in_setting.swapChainFormat);
    swapChainDesc.Stereo                = FALSE;
    swapChainDesc.SampleDesc.Count      = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality    = 0;   // Not use MSAA
    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.BufferCount           = 2;
    swapChainDesc.Scaling               = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags                 = 0;
    swapChain->m_vsync                  = in_setting.enableVSync;
    swapChain->m_backBufferFormat       = in_setting.swapChainFormat;
    swapChain->m_depthBufferFormat      = in_setting.depthBufferFormat;
    swapChain->m_enableHDRRendering     = in_setting.enableHDRRendering;
    swapChain->m_enableMSAA             = in_setting.enableMSAA;

    ComPtr<IDXGIDevice2> dxgiDevice;
    CheckD3D11Result(d->QueryInterface(
                         __uuidof(IDXGIDevice2),
                         reinterpret_cast<void**>(dxgiDevice.GetAddressOf())),
                     "QueryInterface Fail.");

    ComPtr<IDXGIAdapter> dxgiAdapter;
    CheckD3D11Result(dxgiDevice->GetParent(
                         __uuidof(IDXGIAdapter),
                         reinterpret_cast<void**>(dxgiAdapter.GetAddressOf())),
                     "GetParent Fail.");

    ComPtr<IDXGIFactory2> dxgiFactory;
    CheckD3D11Result(dxgiAdapter->GetParent(
                         __uuidof(IDXGIFactory2),
                         reinterpret_cast<void**>(dxgiFactory.GetAddressOf())),
                     "GetParent Fail.");

    CheckD3D11Result(dxgiFactory->CreateSwapChainForHwnd(
                         d,
                         in_hWnd,
                         &swapChainDesc,
                         nullptr,
                         nullptr,
                         swapChain->m_swapChain.GetAddressOf()),
                     "CreateSwapChainForHWnd Fail.");

    swapChain->_CreateResources(in_screenSize);

    return swapChain;
}

void Swapchain::Present() const
{
    static DXGI_PRESENT_PARAMETERS presentParameters = { 0, nullptr, nullptr, nullptr };
    m_swapChain->Present1(m_vsync ? 1 : 0, 0, &presentParameters);
}

void Swapchain::OnResize(Int2 in_size)
{
    m_backBuffer.reset();
    m_backBufferHDR.reset();
    m_depthBuffer.reset();
    m_depthOnlyBuffer.reset();

    CheckD3D11Result(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers Fail.");
    _CreateResources(in_size);
}

void Swapchain::EnableMSAA(bool in_enable)
{
    if (in_enable == m_enableMSAA)
        return;

    if (in_enable)
    {
        auto [width, height] = GetAppWindow().GetWindowSize();

        m_enableMSAA      = true;
        m_MSAASampleCount = 4;

        if (!CheckD3D11Result(GetRenderer().GetDevice()->CheckMultisampleQualityLevels(
                                  DXGI_FORMAT(eFormat::Float16x4),
                                  4,
                                  &m_MSAAQuality),
                              "CheckMultisampleQualityLevels Fail."))
        {
            m_enableMSAA      = false;
            m_MSAASampleCount = 1;
            m_MSAAQuality     = 0;
        }
    }
    else
    {
        m_enableMSAA      = false;
        m_MSAASampleCount = 1;
        m_MSAAQuality     = 0;
    }

    _CreateResources(GetAppWindow().GetWindowSize());
}

Viewport Swapchain::GetViewport() const
{
    return m_viewport;
}

Ref<DepthBuffer> Swapchain::GetDepthBuffer() const
{
    return m_depthBuffer;
}

Ref<DepthBuffer> Swapchain::GetDepthOnlyBuffer() const
{
    return m_depthOnlyBuffer;
}

Ref<Image2D> Swapchain::GetDepthOnlyBufferImage() const
{
    return m_depthOnlyBuffer->GetImage();
}

Ref<RenderTarget> Swapchain::GetBackBuffer() const
{
    return m_backBuffer;
}

Ref<Image2D> Swapchain::GetBackBufferImage() const
{
    return m_backBuffer->GetImage();
}

Ref<RenderTarget> Swapchain::GetBackBufferHDR() const
{
    if (m_enableHDRRendering)
    {
        return m_backBufferHDR;
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<Image2D> Swapchain::GetBackBufferHDRImage() const
{
    if (m_enableHDRRendering)
    {
        return m_backBufferHDR->GetImage();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

Ref<Image2D> Swapchain::GetResolvedBackBufferImage() const
{
    if (m_enableHDRRendering)
    {
        return m_resolvedBackBuffer->GetImage();
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
        return nullptr;
    }
}

void Swapchain::ResolveBackBuffer() const
{
    if (m_enableHDRRendering)
    {
        GetRenderer().GetContext()->ResolveSubresource(
            m_resolvedBackBufferImage.Get(),
            0,
            m_backBufferHDRImage.Get(),
            0,
            static_cast<DXGI_FORMAT>(m_resolvedBackBuffer->GetFormat()));
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
    }
}

void Swapchain::_CreateResources(const Int2& in_size)
{
    auto d = GetRenderer().GetDevice();

    // viewport
    Viewport vp = {};
    vp.width    = static_cast<float>(in_size.x);
    vp.height   = static_cast<float>(in_size.y);
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = 0.f;
    vp.y        = 0.f;
    m_viewport  = vp;

    // SwapChain back buffer
    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr;
    CheckD3D11Result(m_swapChain->GetBuffer(0,
                                            __uuidof(ID3D11Texture2D),
                                            reinterpret_cast<void**>(backBufferTexture.GetAddressOf())),
                     "GetBuffer Fail.");

    m_backBuffer = RenderTarget::Create(backBufferTexture.Get());

    // HDR Resources
    _CreateHDRRenderTarget(in_size);

    // Depth stencil
    m_depthBuffer = DepthBuffer::Create(
        in_size.x,
        in_size.y,
        m_depthBufferFormat,
        m_MSAASampleCount,
        m_MSAAQuality);

    m_depthOnlyBuffer = DepthBuffer::CreateWithImage2D(
        in_size.x,
        in_size.y,
        eFormat::Depth_Float32,
        m_MSAASampleCount,
        m_MSAAQuality,
        eFormat::Float32);
}

void Swapchain::_CreateHDRRenderTarget(const Int2& in_size)
{
    auto d = GetRenderer().GetDevice();

    m_backBufferHDRImage = ID3D11Texture2DUtil::CreateTexture2D(
        in_size.x,
        in_size.y,
        eFormat::Float16x4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0,
        m_MSAASampleCount,
        m_MSAAQuality);

    m_backBufferHDR = RenderTarget::Create(m_backBufferHDRImage.Get());

    m_resolvedBackBufferImage = ID3D11Texture2DUtil::CreateTexture2D(
        in_size.x,
        in_size.y,
        eFormat::Float16x4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0,
        1,
        0);

    m_resolvedBackBuffer = RenderTarget::Create(m_resolvedBackBufferImage.Get());
}

}   // namespace crab