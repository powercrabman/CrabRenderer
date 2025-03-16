#include "CrabPch.h"

#include "Swapchain.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Textures.h"

namespace crab
{

void Swapchain::Init(
    const SwapChainSetting& in_setting,
    const Int2&             in_screenSize,
    HWND                    in_hWnd)
{
    auto d = GetRenderer().GetDevice();

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
    m_vsync                             = in_setting.enableVSync;
    m_backBufferFormat                  = in_setting.swapChainFormat;
    m_depthBufferFormat                 = in_setting.depthBufferFormat;
    m_enableHDRRendering                = in_setting.enableHDRRendering;
    m_enableMSAA                        = in_setting.enableMSAA;

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
                         m_swapChain.GetAddressOf()),
                     "CreateSwapChainForHWnd Fail.");

    _InitMSAA();
    _CreateResources(in_screenSize);
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

    CheckD3D11Result(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers Fail.");
    _CreateResources(in_size);
}

void Swapchain::_InitMSAA()
{
    m_MSAASampleCount = UseMSAA() ? 4 : 1;
    m_MSAAQuality     = 0;

    if (m_enableMSAA)
    {

        uint32 maxQuality = 0;
        if (CheckD3D11Result(GetRenderer().GetDevice()->CheckMultisampleQualityLevels(
                                 static_cast<DXGI_FORMAT>(eFormat::Float16x4),
                                 m_MSAASampleCount,
                                 &maxQuality),
                             "CheckMultisampleQualityLevels Fail."))
        {
            m_MSAAQuality = maxQuality - 1;
        }
    }
}

Ref<DepthBuffer> Swapchain::GetDepthBuffer() const
{
    return m_depthBuffer;
}

Ref<RenderTarget> Swapchain::GetBackBuffer() const
{
    return m_backBuffer;
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

void Swapchain::_CreateResources(const Int2& in_size)
{
    auto d = GetRenderer().GetDevice();

    // SwapChain back buffer
    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr;
    CheckD3D11Result(m_swapChain->GetBuffer(0,
                                            __uuidof(ID3D11Texture2D),
                                            reinterpret_cast<void**>(backBufferTexture.GetAddressOf())),
                     "GetBuffer Fail.");

    m_backBuffer = CreateRef<RenderTarget>();
    m_backBuffer->Init(backBufferTexture.Get());

    // Depth stencil
    m_depthBuffer = CreateRef<DepthBuffer>();
    m_depthBuffer->Init(in_size.x, in_size.y, m_depthBufferFormat, m_MSAASampleCount, m_MSAAQuality);

    if (IsHDR())
    {
        // HDR Resources
        _CreateHDRRenderTarget(in_size);
    }
}

void Swapchain::_CreateHDRRenderTarget(const Int2& in_size)
{
    ComPtr<ID3D11Texture2D> texture = ID3D11Texture2DUtil::CreateTexture2D(
        in_size.x,
        in_size.y,
        eFormat::Float16x4,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0,
        m_MSAASampleCount,
        m_MSAAQuality);

    m_backBufferHDR = CreateRef<RenderTarget>();
    m_backBufferHDR->Init(texture.Get());
}

}   // namespace crab