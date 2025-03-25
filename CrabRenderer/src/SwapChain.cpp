#include "InternalPCH.h"

#include "SwapChain.h"

#include "DepthBuffer.h"
#include "FrameBuffer.h"
#include "RenderFactory.h"
#include "RenderTarget.h"
#include "Renderer.h"

namespace crab
{

void SwapChain::Init(const RendererInitInfo& in_initInfo)
{
    auto d = GetRenderer().GetDevice();

    // - Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width              = in_initInfo.width;
    swapChainDesc.Height             = in_initInfo.height;
    swapChainDesc.Format             = d3d11::GetFormat(in_initInfo.backBufferFormat);
    swapChainDesc.Stereo             = FALSE;
    swapChainDesc.SampleDesc.Count   = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality = 0;   // Not use MSAA
    swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.BufferCount        = 2;
    swapChainDesc.Scaling            = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags              = 0;

    ComPtr<IDXGIDevice2> dxgiDevice;
    d3d11::CheckOK(d->QueryInterface(
                       __uuidof(IDXGIDevice2),
                       reinterpret_cast<void**>(dxgiDevice.GetAddressOf())),
                   "QueryInterface Fail.");

    ComPtr<IDXGIAdapter> dxgiAdapter;
    d3d11::CheckOK(dxgiDevice->GetParent(
                       __uuidof(IDXGIAdapter),
                       reinterpret_cast<void**>(dxgiAdapter.GetAddressOf())),
                   "GetParent Fail.");

    ComPtr<IDXGIFactory2> dxgiFactory;
    d3d11::CheckOK(dxgiAdapter->GetParent(
                       __uuidof(IDXGIFactory2),
                       reinterpret_cast<void**>(dxgiFactory.GetAddressOf())),
                   "GetParent Fail.");

    d3d11::CheckOK(dxgiFactory->CreateSwapChainForHwnd(
                       d,
                       in_initInfo.nativeHandle,
                       &swapChainDesc,
                       nullptr,
                       nullptr,
                       m_swapChain.GetAddressOf()),
                   "CreateSwapChainForHWnd Fail.");

    CreateResources_Internal(in_initInfo.width,
                             in_initInfo.height,
                             in_initInfo.createDepthBuffer,
                             in_initInfo.depthBufferFormat,
                             in_initInfo.createHDR,
                             in_initInfo.HDRBackBufferFormat);
}

void SwapChain::Present(bool in_vsync) const
{
    static DXGI_PRESENT_PARAMETERS presentParameters = { 0, nullptr, nullptr, nullptr };
    m_swapChain->Present1(in_vsync ? 1 : 0, 0, &presentParameters);
}

void SwapChain::OnResize(uint32 in_width, uint32 in_height)
{
    bool createHDRFrameBuffer = m_HDRFrameBuffer != nullptr;
    bool createDepthBuffer    = m_depthBuffer != nullptr;

    eFormat HDRFormat         = eFormat::Unknown;
    eFormat depthBufferFormat = eFormat::Unknown;

    if (m_HDRFrameBuffer)
        HDRFormat = m_HDRFrameBuffer->GetRenderTarget(0)->GetFormat();

    if (createDepthBuffer)
        depthBufferFormat = m_depthBuffer->GetFormat();

    // if resize buffer fail, reset all back buffer oriented resources

    m_backBufferFO.reset();

    d3d11::CheckOK(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0),
                   "ResizeBuffers Fail.");

    CreateResources_Internal(in_width,
                             in_height,
                             createDepthBuffer,
                             depthBufferFormat,
                             createHDRFrameBuffer,
                             HDRFormat);
}

Ref<FrameBuffer> SwapChain::GetBackBufferFB() const
{
    ASSERT(m_backBufferFO, "SwapChain::GetBackBufferFB: m_backBufferFO is nullptr.");
    return m_backBufferFO;
}

Ref<FrameBuffer> SwapChain::GetHDRBackBufferFB() const
{
    ASSERT(m_HDRFrameBuffer, "SwapChain::GetHDRBackBufferFB: m_HDRFrameBuffer is nullptr.");
    return m_HDRFrameBuffer;
}

Ref<DepthBuffer> SwapChain::GetDepthBuffer() const
{
    ASSERT(m_depthBuffer, " SwapChain::GetDepthBuffer: m_depthBuffer is nullptr.");
    return m_depthBuffer;
}

void SwapChain::CreateResources_Internal(uint32  in_width,
                                         uint32  in_height,
                                         bool    in_createDepthBuffer,
                                         eFormat in_depthBufferFormat,
                                         bool    in_createHDR,
                                         eFormat in_HDRFormat)
{
    MSAA msaa = GetRenderer().GetMSAA();

    // SwapChain back buffer
    ComPtr<ID3D11Texture2D> backBufferTexture = nullptr;
    d3d11::CheckOK(m_swapChain->GetBuffer(0,
                                          __uuidof(ID3D11Texture2D),
                                          reinterpret_cast<void**>(backBufferTexture.GetAddressOf())),
                   "GetBuffer Fail.");

    // create back buffer render target

    Ref<RenderTarget> renderTargets = CreateRef<RenderTarget>();
    renderTargets->Init(backBufferTexture.Get());

    // create depth buffer

    m_depthBuffer.reset();

    if (in_createDepthBuffer)
    {
        m_depthBuffer = factory::CreateDepthBuffer(
            in_width,
            in_height,
            in_depthBufferFormat,
            msaa,
            1,
            eTextureCreationFlags_None);
    }

    // create main frame buffer

    m_backBufferFO = CreateRef<FrameBuffer>();
    m_backBufferFO->Init({ renderTargets }, m_depthBuffer);

    // optional create HDR back buffer

    if (in_createHDR)
    {
        CreateHDRRenderTarget_Internal(in_width, in_height, in_HDRFormat);
    }
}

void SwapChain::CreateHDRRenderTarget_Internal(uint32  in_width,
                                               uint32  in_height,
                                               eFormat in_HDRFormat)
{
    MSAA msaa = GetRenderer().GetMSAA();

    m_HDRFrameBuffer = factory::CreateFrameBuffer(
        {
            factory::CreateRenderTarget(
                in_width,
                in_height,
                in_HDRFormat,
                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
                msaa),
        },
        m_depthBuffer);
}

}   // namespace crab