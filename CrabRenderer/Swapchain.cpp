#include "CrabPch.h"

#include "SwapChain.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Textures.h"

namespace crab
{

void SwapChain::Init(
    const RendererSetting& in_setting,
    const Int2&            in_screenSize,
    MSAA                   in_msaa,
    HDR                    in_hdr,
    HWND                   in_hWnd)
{
    auto d = GetRenderer().GetDevice();

    // - Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width              = in_screenSize.x;
    swapChainDesc.Height             = in_screenSize.y;
    swapChainDesc.Format             = static_cast<DXGI_FORMAT>(in_setting.backBufferFormat);
    swapChainDesc.Stereo             = FALSE;
    swapChainDesc.SampleDesc.Count   = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality = 0;   // Not use MSAA
    swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.BufferCount        = 2;
    swapChainDesc.Scaling            = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags              = 0;
    m_backBufferFormat               = in_setting.backBufferFormat;
    m_depthBufferFormat              = in_setting.backBufferDepthFormat;

    ComPtr<IDXGIDevice2> dxgiDevice;
    d3d::CheckOK(d->QueryInterface(
                     __uuidof(IDXGIDevice2),
                     reinterpret_cast<void**>(dxgiDevice.GetAddressOf())),
                 "QueryInterface Fail.");

    ComPtr<IDXGIAdapter> dxgiAdapter;
    d3d::CheckOK(dxgiDevice->GetParent(
                     __uuidof(IDXGIAdapter),
                     reinterpret_cast<void**>(dxgiAdapter.GetAddressOf())),
                 "GetParent Fail.");

    ComPtr<IDXGIFactory2> dxgiFactory;
    d3d::CheckOK(dxgiAdapter->GetParent(
                     __uuidof(IDXGIFactory2),
                     reinterpret_cast<void**>(dxgiFactory.GetAddressOf())),
                 "GetParent Fail.");

    d3d::CheckOK(dxgiFactory->CreateSwapChainForHwnd(
                     d,
                     in_hWnd,
                     &swapChainDesc,
                     nullptr,
                     nullptr,
                     m_swapChain.GetAddressOf()),
                 "CreateSwapChainForHWnd Fail.");

    _CreateResources(in_screenSize);
}

void SwapChain::Present(bool in_vsync) const
{
    static DXGI_PRESENT_PARAMETERS presentParameters = { 0, nullptr, nullptr, nullptr };
    m_swapChain->Present1(in_vsync ? 1 : 0, 0, &presentParameters);
}

void SwapChain::OnResize(Int2 in_size)
{
    m_backBuffer.reset();
    m_backBufferHDR.reset();
    m_depthBuffer.reset();

    d3d::CheckOK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers Fail.");
    _CreateResources(in_size);
}

Ref<DepthBuffer> SwapChain::GetDepthBuffer() const
{
    return m_depthBuffer;
}

Ref<RenderTarget> SwapChain::GetBackBuffer() const
{
    return m_backBuffer;
}

Ref<RenderTarget> SwapChain::GetBackBufferHDR() const
{
    return m_backBufferHDR;
}

void SwapChain::_CreateResources(const Int2& in_size)
{
    auto d = GetRenderer().GetDevice();
    auto r = GetRenderer().GetContext();

    MSAA msaa = GetRenderer().GetMSAA();
    HDR  hdr  = GetRenderer().GetHDR();

    // SwapChain back buffer
    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr;
    d3d::CheckOK(m_swapChain->GetBuffer(0,
                                        __uuidof(ID3D11Texture2D),
                                        reinterpret_cast<void**>(backBufferTexture.GetAddressOf())),
                 "GetBuffer Fail.");

    m_backBuffer = CreateRef<RenderTarget>();
    m_backBuffer->Init(backBufferTexture.Get());

    // Depth stencil
    m_depthBuffer = CreateRef<DepthBuffer>();
    m_depthBuffer->Init(in_size.x, in_size.y, m_depthBufferFormat, msaa);

    if (hdr.enableHDR)
    {
        // HDR Resources
        _CreateHDRRenderTarget(in_size);
    }
}

void SwapChain::_CreateHDRRenderTarget(const Int2& in_size)
{
    MSAA msaa = GetRenderer().GetMSAA();

    ComPtr<ID3D11Texture2D> texture = ID3D11Texture2DUtil::CreateTexture2D(
        in_size.x,
        in_size.y,
        eFormat::Float16_4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        eCPUAccessFlags_None,
        msaa,
        1,
        1,
        eTextureCreationFlags_None,
        nullptr);

    m_backBufferHDR = CreateRef<RenderTarget>();
    m_backBufferHDR->Init(texture.Get());
}

}   // namespace crab