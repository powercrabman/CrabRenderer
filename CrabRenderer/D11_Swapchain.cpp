#include "CrabPch.h"

#include "D11_Swapchain.h"

#include "AppWindow.h"
#include "D11_DepthStencil.h"
#include "D11_FrameBuffer.h"
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
    swapchain->m_useDepthStencil        = in_setting.useDepthStencil;

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

void D11_Swapchain::BindBackBuffer() const
{
    m_backbuffer->Bind();
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
    m_backbuffer.reset();

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

void D11_Swapchain::Clear(const Color& in_color, bool in_clearDepth, bool in_clearStencil)
{
    m_backbuffer->Clear(in_color, in_clearDepth, in_clearStencil);
}

crab::Ref<crab::D11_Texture> D11_Swapchain::GetBackBufferTexture() const
{
    return m_backbuffer->GetRenderTarget()->GetTexture();
}

void D11_Swapchain::_CreateResources(uint32 in_width, uint32 in_height)
{
    auto d = D11_API->GetDevice();

    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr; 
    D11_ASSERT(m_swapChain->GetBuffer(0,
                                      __uuidof(ID3D11Texture2D),
                                      (void**)backBufferTexture.GetAddressOf()),
               "GetBuffer Fail.");

    Ref<D11_RenderTarget> rt = D11_RenderTarget::Create(backBufferTexture.Get());

    // depth stencil
    Ref<D11_DepthStencil> ds = nullptr;

    if (m_useDepthStencil)
        ds = D11_DepthStencil::Create(m_depthStencilFormat);

    // viewport
    Viewport vp = {};
    vp.width    = (float)in_width;
    vp.height   = (float)in_height;
    vp.minDepth = 0.f;
    vp.maxDepth = 1.f;
    vp.x        = 0.f;
    vp.y        = 0.f;

    // framebuffer
    m_backbuffer = D11_FrameBuffer::Create(rt, vp, ds);
}

}   // namespace crab