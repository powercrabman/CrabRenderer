#include "CrabPch.h"

#include "AppWindow.h"
#include "D11_Imgui.h"
#include "D11_Renderer.h"

#include "CrabEvent.h"
#include "D11_DepthStencil.h"
#include "D11_FrameBuffer.h"
#include "D11_RenderTarget.h"
#include "EventDispatcher.h"

#include "D11_Buffers.h"
#include "D11_ConstBuffer.h"
#include "D11_RenderState.h"
#include "D11_Shaders.h"
#include "Texture.h"

namespace crab
{

D11_Renderer::D11_Renderer()  = default;
D11_Renderer::~D11_Renderer() = default;

void D11_Renderer::Init(const RendererSetting& in_setting)
{
    // - Device & Device Context & Swap Chain
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // - Create Device

    D11_ASSERT(D3D11CreateDevice(nullptr,
                                 D3D_DRIVER_TYPE_HARDWARE,
                                 NULL,
                                 createDeviceFlags,
                                 featureLevels,
                                 ARRAYSIZE(featureLevels),
                                 D3D11_SDK_VERSION,
                                 m_device.GetAddressOf(),
                                 &m_featureLevel,
                                 m_deviceContext.GetAddressOf()),
               "D3D11CreateDevice Fail.");

    // - Swap Chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width                 = 0;
    swapChainDesc.Height                = 0;
    swapChainDesc.Format                = in_setting.swapChainFrameBufferSetting.format;
    swapChainDesc.Stereo                = FALSE;
    swapChainDesc.SampleDesc.Count      = 1;   // Not use MSAA
    swapChainDesc.SampleDesc.Quality    = 0;   // Not use MSAA
    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount           = 2;
    swapChainDesc.Scaling               = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode             = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags                 = 0;

    m_vsync = in_setting.useVSync;

    ComPtr<IDXGIDevice2> dxgiDevice;
    D11_ASSERT(m_device->QueryInterface(__uuidof(IDXGIDevice2), (void**)dxgiDevice.GetAddressOf()), "QueryInterface Fail.");

    ComPtr<IDXGIAdapter> dxgiAdapter;
    D11_ASSERT(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.GetAddressOf()), "GetParent Fail.");

    ComPtr<IDXGIFactory2> dxgiFactory;
    D11_ASSERT(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.GetAddressOf()), "GetParent Fail.");

    D11_ASSERT(dxgiFactory->CreateSwapChainForHwnd(m_device.Get(),
                                                   (HWND)GetApplication().GetAppWindow().GetNativeWindowHandle(),
                                                   &swapChainDesc,
                                                   nullptr,
                                                   nullptr,
                                                   m_swapChain.GetAddressOf()),
               "CreateSwapChainForHwnd Fail.");

#ifdef _DEBUG
    // - debug
    D11_ASSERT(m_device->QueryInterface(IID_PPV_ARGS(m_debug.GetAddressOf())), "QueryInterface Fail.");
    D11_ASSERT(m_debug->QueryInterface(IID_PPV_ARGS(m_infoQueue.GetAddressOf())), "QueryInterface Fail.");
#endif

    // imgui
    m_imgui = CreateScope<D11_Imgui>();
    m_imgui->Init();

    // FrameBuffer
    Viewport viewport    = in_setting.swapChainFrameBufferSetting.viewport;
    auto [width, height] = GetApplication().GetAppWindow().GetWindowSize();
    if (viewport.width == 0)
        viewport.width = (float)width;
    if (viewport.height == 0)
        viewport.height = (float)height;

    Ref<D11_DepthStencil> depthStencil = in_setting.swapChainFrameBufferSetting.useDepthStencil ?
                                             D11_DepthStencil::CreateFromSwapChain(in_setting.swapChainFrameBufferSetting.depthStencilFormat) :
                                             nullptr;

    m_swapChainBackbuffer = D11_FrameBuffer::Create(
        D11_RenderTarget::CreateFromSwapChain(),
        viewport,
        depthStencil);

    // default resources
    SetFrameBuffer(m_swapChainBackbuffer);
    m_defaultSamplerState = D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Clamp);

    // Pipeline State
    m_vsConstantBuffers.fill(nullptr);
    m_psConstantBuffers.fill(nullptr);
}

void D11_Renderer::Shutdown()
{
    m_imgui->Shutdown();
}

void D11_Renderer::OnEvent(CrabEvent& in_event)
{
    EventDispatcher dispatcher(in_event);

    HANDLE_EVENT(Resize_WindowEvent,
                 [&](const Resize_WindowEvent& e)
                 {
                     Viewport viewport = m_swapChainBackbuffer->viewport;

                     m_swapChainBackbuffer.reset();
                     D11_ASSERT(m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0),
                                "ResizeBuffers Fail.");

                     viewport.width  = (float)e.width;
                     viewport.height = (float)e.height;

                     m_swapChainBackbuffer = D11_FrameBuffer::Create(
                         { D11_RenderTarget::CreateFromSwapChain() },
                         viewport,
                         m_swapChainBackbuffer->depthStencil);
                 });
}

void D11_Renderer::Draw(uint32 in_vertexCount, uint32 in_startVertexLocation /*= 0*/)
{
    m_deviceContext->Draw(in_vertexCount, in_startVertexLocation);
}

void D11_Renderer::DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation /*= 0*/, int32 in_baseVertexLocation /*= 0*/)
{
    m_deviceContext->DrawIndexed(in_indexCount, in_startIndexLocation, in_baseVertexLocation);
}

void D11_Renderer::SetTopology(eTopology in_topology)
{
    if (m_topology != in_topology)
    {
        m_topology = in_topology;
        m_deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)in_topology);
    }
}

void D11_Renderer::SetVertexBuffer(const Ref<D11_VertexBuffer>& in_vertexBuffer)
{
    if (m_vertexBuffer != in_vertexBuffer)
    {
        m_vertexBuffer = in_vertexBuffer;
        m_deviceContext->IASetVertexBuffers(0,
                                            1,
                                            m_vertexBuffer->buffer.GetAddressOf(),
                                            &m_vertexBuffer->vertexStride,
                                            &m_vertexBuffer->offset);
    }
}

void D11_Renderer::SetIndexBuffer(const Ref<D11_IndexBuffer>& in_indexBuffer)
{
    if (m_indexBuffer != in_indexBuffer)
    {
        m_indexBuffer = in_indexBuffer;
        m_deviceContext->IASetIndexBuffer(m_indexBuffer->buffer.Get(),
                                          DXGI_FORMAT_R32_UINT,
                                          0);
    }
}

void D11_Renderer::SetVertexShader(const Ref<D11_VertexShader>& in_vertexShader)
{
    if (m_vertesShader != in_vertexShader)
    {
        m_vertesShader = in_vertexShader;
        m_deviceContext->VSSetShader(m_vertesShader->vertexShader.Get(), nullptr, 0);
        m_deviceContext->IASetInputLayout(m_vertesShader->inputLayout.Get());
    }
}

void D11_Renderer::SetPixelShader(const Ref<D11_PixelShader>& in_pixelShader)
{
    if (m_pixelShader != in_pixelShader)
    {
        m_pixelShader = in_pixelShader;
        m_deviceContext->PSSetShader(m_pixelShader->pixelShader.Get(), nullptr, 0);
    }
}

void D11_Renderer::SetRasterizerState(const Ref<D11_RasterizerState>& in_rasterizerState)
{
    if (m_rsState != in_rasterizerState)
    {
        m_rsState = in_rasterizerState;
        m_deviceContext->RSSetState(m_rsState->rasterizerState.Get());
    }
}

void D11_Renderer::SetDepthStencilState(const Ref<D11_DepthStencilState>& in_depthStencilState)
{
    if (m_dsState != in_depthStencilState)
    {
        m_dsState = in_depthStencilState;
        m_deviceContext->OMSetDepthStencilState(m_dsState->depthStencilState.Get(), 0);
    }
}

void D11_Renderer::SetBlendState(const Ref<D11_BlendState>& in_blendState)
{
    if (m_bState != in_blendState)
    {
        m_bState = in_blendState;
        m_deviceContext->OMSetBlendState(m_bState->blendState.Get(), nullptr, 0xffffffff);
    }
}

void D11_Renderer::SetTexture(const Ref<Texture>& in_texture, uint32 in_slot, eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        if (in_texture)
        {
            m_deviceContext->VSSetShaderResources(in_slot, 1, in_texture->srv.GetAddressOf());
            m_deviceContext->VSSetSamplers(in_slot, 1, in_texture->samplerState->samplerState.GetAddressOf());
        }
        else
        {
            ID3D11ShaderResourceView* nullSRV = nullptr;
            m_deviceContext->VSSetShaderResources(in_slot, 1, &nullSRV);
            m_deviceContext->VSSetSamplers(in_slot, 1, m_defaultSamplerState->samplerState.GetAddressOf());
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        if (in_texture)
        {
            m_deviceContext->PSSetShaderResources(in_slot, 1, in_texture->srv.GetAddressOf());
            m_deviceContext->PSSetSamplers(in_slot, 1, in_texture->samplerState->samplerState.GetAddressOf());
        }
        else
        {
            ID3D11ShaderResourceView* nullSRV = nullptr;
            m_deviceContext->PSSetShaderResources(in_slot, 1, &nullSRV);
            m_deviceContext->PSSetSamplers(in_slot, 1, m_defaultSamplerState->samplerState.GetAddressOf());
        }
    }
}

void D11_Renderer::SetTextureArray(const TextureArray& in_textures, uint32 in_startSlot, eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        m_deviceContext->VSSetShaderResources(in_startSlot, (UINT)in_textures.srvPtrs.size(), in_textures.srvPtrs.data());
        m_deviceContext->VSSetSamplers(in_startSlot, (UINT)in_textures.statePtrs.size(), in_textures.statePtrs.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        m_deviceContext->PSSetShaderResources(in_startSlot, (UINT)in_textures.srvPtrs.size(), in_textures.srvPtrs.data());
        m_deviceContext->PSSetSamplers(in_startSlot, (UINT)in_textures.statePtrs.size(), in_textures.statePtrs.data());
    }
}

void D11_Renderer::SetConstantBuffer(const Ref<D11_ConstantBufferBase>& in_constantBuffer, uint32 in_slot, eShaderFlags in_flag)
{
    if (in_flag & eShaderFlags_VertexShader)
    {
        if (m_vsConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_vsConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->VSSetConstantBuffers(in_slot, 1, in_constantBuffer->buffer.GetAddressOf());
        }
    }
    if (in_flag & eShaderFlags_PixelShader)
    {
        if (m_psConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_psConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->PSSetConstantBuffers(in_slot, 1, in_constantBuffer->buffer.GetAddressOf());
        }
    }
}

void D11_Renderer::SetFrameBuffer(const Ref<D11_FrameBuffer>& in_frameBuffer)
{
    m_frameBuffer = in_frameBuffer;
    m_deviceContext->OMSetRenderTargets(1,
                                        m_frameBuffer->renderTarget->renderTargetView.GetAddressOf(),
                                        m_frameBuffer->depthStencil ? m_frameBuffer->depthStencil->dsv.Get() : nullptr);
    m_deviceContext->RSSetViewports(1, m_frameBuffer->viewport.Get11());
}

void D11_Renderer::SetBackBufferToFrameBuffer()
{
    SetFrameBuffer(m_swapChainBackbuffer);
}

void D11_Renderer::ClearFrameBuffer(const Color& in_color, bool in_clearDepth, bool in_clearStencil)
{
    CRAB_ASSERT(m_frameBuffer, "FrameBuffer is nullptr.");

    // - Clear Render Target View
    m_deviceContext->ClearRenderTargetView(m_frameBuffer->renderTarget->renderTargetView.Get(),
                                           (FLOAT*)&in_color);

    // - Clear Depth Stencil
    if (m_frameBuffer->depthStencil)
    {
        UINT clearFlags = 0;

        if (in_clearDepth)
            clearFlags |= D3D11_CLEAR_DEPTH;

        if (in_clearStencil)
            clearFlags |= D3D11_CLEAR_STENCIL;

        m_deviceContext->ClearDepthStencilView(m_frameBuffer->depthStencil->dsv.Get(), clearFlags, 1.f, 0);
    }
}

void D11_Renderer::ClearFrameBuffer(const Color& in_color)
{
    ClearFrameBuffer(in_color, true, true);
}

void D11_Renderer::BeginRender()
{
    m_imgui->BeginRender();
}

void D11_Renderer::EndRender()
{
    m_imgui->EndRender();
}

void D11_Renderer::Present()
{
    static DXGI_PRESENT_PARAMETERS presentParameters = {};
    presentParameters.DirtyRectsCount                = 0;
    presentParameters.pDirtyRects                    = nullptr;
    presentParameters.pScrollRect                    = nullptr;
    presentParameters.pScrollOffset                  = nullptr;

    m_swapChain->Present1(m_vsync ? 1 : 0, 0, &presentParameters);
}

crab::ComPtr<ID3D11Device> D11_Renderer::GetDevice() const
{
    return m_device;
}

crab::ComPtr<ID3D11DeviceContext> D11_Renderer::GetContext() const
{
    return m_deviceContext;
}

crab::ComPtr<IDXGISwapChain> D11_Renderer::GetSwapChain() const
{
    return m_swapChain;
}

crab::Ref<crab::D11_FrameBuffer> D11_Renderer::GetSwapChainFrameBuffer() const
{
    return m_swapChainBackbuffer;
}

}   // namespace crab
