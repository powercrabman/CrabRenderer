#include "CrabPch.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "ImguiVendor.h"

#include "CrabEvent.h"
#include "DepthBuffer.h"
#include "EventDispatcher.h"
#include "CommonState.h"
#include "RenderTarget.h"
#include "Swapchain.h"

namespace crab
{

D11Renderer::D11Renderer()
{
}

D11Renderer::~D11Renderer() = default;

void D11Renderer::Init(const RendererSetting& in_setting)
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

    CheckD3D11Result(D3D11CreateDevice(nullptr,
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
    m_swapChain = Swapchain::Create(in_setting.swapChainSetting);

#ifdef _DEBUG
    // - debug
    CheckD3D11Result(m_device->QueryInterface(IID_PPV_ARGS(m_debug.GetAddressOf())), "QueryInterface Fail.");
    CheckD3D11Result(m_debug->QueryInterface(IID_PPV_ARGS(m_infoQueue.GetAddressOf())), "QueryInterface Fail.");
#endif

    // imgui
    ImguiVendor::Init();

    // Render State Storage
    m_renderStateStorage = CreateScope<CommonState>();
    m_renderStateStorage->Init();
}

void D11Renderer::Shutdown()
{
    ImguiVendor::Shutdown();
}

void D11Renderer::OnEvent(CrabEvent& in_event) const
{
    EventDispatcher dispatcher(in_event);

    HANDLE_EVENT(Resize_WindowEvent,
                 [&](const Resize_WindowEvent& e)
                 {
                     m_swapChain->OnResize(e.width, e.height);
                 });
}

void D11Renderer::Draw(uint32 in_vertexCount, uint32 in_startVertexLocation /*= 0*/) const
{
    m_deviceContext->Draw(in_vertexCount, in_startVertexLocation);
}

void D11Renderer::DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation /*= 0*/, int32 in_baseVertexLocation /*= 0*/) const
{
    m_deviceContext->DrawIndexed(in_indexCount, in_startIndexLocation, in_baseVertexLocation);
}

void D11Renderer::SetTopology(eTopology in_topology)
{
    D3D11_PRIMITIVE_TOPOLOGY topology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(in_topology);
    if (m_topology != topology)
    {
        m_topology = topology;
        m_deviceContext->IASetPrimitiveTopology(topology);
    }
}

void D11Renderer::SetVertexBuffer(ID3D11Buffer* in_vertexBuffer, uint32 in_stride, uint32 in_offset)
{
    if (m_vertexBuffer != in_vertexBuffer)
    {
        m_vertexBuffer = in_vertexBuffer;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &in_stride, &in_offset);
    }
}

void D11Renderer::SetIndexBuffer(ID3D11Buffer* in_indexBuffer)
{
    if (m_indexBuffer != in_indexBuffer)
    {
        m_indexBuffer = in_indexBuffer;
        m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }
}

void D11Renderer::SetVertexShader(ID3D11VertexShader* in_shader, ID3D11InputLayout* in_layout)
{
    if (m_vertexShader != in_shader)
    {
        m_vertexShader = in_shader;
        m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
        m_deviceContext->IASetInputLayout(in_layout);
    }
}
void D11Renderer::SetPixelShader(ID3D11PixelShader* in_shader)
{
    if (m_pixelShader != in_shader)
    {
        m_pixelShader = in_shader;
        m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
    }
}

void D11Renderer::SetGeometryShader(ID3D11GeometryShader* in_shader)
{
    if (m_geometryShader != in_shader)
    {
        m_geometryShader = in_shader;
        m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
    }
}

void D11Renderer::SetHullShader(ID3D11HullShader* in_shader)
{
    if (m_hullShader != in_shader)
    {
        m_hullShader = in_shader;
        m_deviceContext->HSSetShader(m_hullShader, nullptr, 0);
    }
}

void D11Renderer::SetDomainShader(ID3D11DomainShader* in_shader)
{
    if (m_domainShader != in_shader)
    {
        m_domainShader = in_shader;
        m_deviceContext->DSSetShader(m_domainShader, nullptr, 0);
    }
}

void D11Renderer::SetSamplerState(
    ID3D11SamplerState* in_srv,
    uint32              in_slot,
    eShaderFlags        in_bindFlags)
{
    if (BIT_AND(in_bindFlags, eShaderFlags_VertexShader))
    {
        if (m_vsSampler[in_slot] != in_srv)
        {
            m_vsSampler[in_slot] = in_srv;
            m_deviceContext->VSSetSamplers(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_HullShader))
    {
        if (m_hsSampler[in_slot] != in_srv)
        {
            m_hsSampler[in_slot] = in_srv;
            m_deviceContext->HSSetSamplers(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_DomainShader))
    {
        if (m_dsSampler[in_slot] != in_srv)
        {
            m_dsSampler[in_slot] = in_srv;
            m_deviceContext->DSSetSamplers(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_GeometryShader))
    {
        if (m_gsSampler[in_slot] != in_srv)
        {
            m_gsSampler[in_slot] = in_srv;
            m_deviceContext->GSSetSamplers(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_PixelShader))
    {
        if (m_psSampler[in_slot] != in_srv)
        {
            m_psSampler[in_slot] = in_srv;
            m_deviceContext->PSSetSamplers(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_ComputeShader))
    {
        if (m_csSampler[in_slot] != in_srv)
        {
            m_csSampler[in_slot] = in_srv;
            m_deviceContext->CSSetSamplers(in_slot, 1, &in_srv);
        }
    }
}

void D11Renderer::SetRasterizerState(ID3D11RasterizerState* in_rasterizerState)
{
    if (m_rsState != in_rasterizerState)
    {
        m_rsState = in_rasterizerState;
        m_deviceContext->RSSetState(m_rsState);
    }
}

void D11Renderer::SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState, uint32 in_stencilRef)
{
    if (m_dsState != in_depthStencilState || m_stencilRef != in_stencilRef)
    {
        m_dsState    = in_depthStencilState;
        m_stencilRef = in_stencilRef;
        m_deviceContext->OMSetDepthStencilState(m_dsState, m_stencilRef);
    }
}

void D11Renderer::SetBlendState(ID3D11BlendState* in_blendState, const std::array<float, 4>& in_blendFactor)
{
    if (m_bState != in_blendState || m_blendFactor != in_blendFactor)
    {
        m_bState      = in_blendState;
        m_blendFactor = in_blendFactor;
        m_deviceContext->OMSetBlendState(m_bState, m_blendFactor.data(), 0xFFFFFFFF);
    }
}

void D11Renderer::SetRenderTarget(ID3D11RenderTargetView* in_renderTargetView, ID3D11DepthStencilView* in_depthStencilView) const
{
    m_deviceContext->OMSetRenderTargets(1, &in_renderTargetView, in_depthStencilView);
}

void D11Renderer::SetRenderTargets(const std::vector<ID3D11RenderTargetView*>& in_renderTargetViews, ID3D11DepthStencilView* in_depthStencilView) const
{
    m_deviceContext->OMSetRenderTargets((UINT)in_renderTargetViews.size(), in_renderTargetViews.data(), in_depthStencilView);
}

void D11Renderer::ReleaseRenderTargets() const
{
    std::array<ID3D11RenderTargetView*, 8> nullRTVs = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    m_deviceContext->OMSetRenderTargets((UINT)nullRTVs.size(), nullRTVs.data(), nullptr);
}

void D11Renderer::SetViewport(const Viewport& in_viewport)
{
    if (m_viewport != in_viewport)
    {
        m_viewport = in_viewport;
        m_deviceContext->RSSetViewports(1, in_viewport.Get11());
    }
}

void D11Renderer::BindBackBuffer(const Ref<DepthBuffer>& in_depthBuffer_or_null)
{
    auto rtv = m_swapChain->GetBackBuffer();
    SetViewport(m_swapChain->GetViewport());

    SetRenderTarget(rtv->Get(),
                    in_depthBuffer_or_null ? in_depthBuffer_or_null->Get() : nullptr);
}

void D11Renderer::ClearBackBuffer(const Color& in_color) const
{
    m_swapChain->GetBackBuffer()->Clear(in_color);
}

void D11Renderer::ClearDepthBuffer(
    bool  in_clearDepth,
    float in_clearDepthFactor,
    bool  in_clearStencil,
    int32 in_clearStencilFactor) const
{
    if (auto buffer = GetDepthBuffer())
        buffer->Clear(
            in_clearDepth,
            in_clearDepthFactor,
            in_clearStencil,
            in_clearStencilFactor);
}

void D11Renderer::ClearDepthOnlyBuffer(
    bool  in_clearDepth,
    float in_clearDepthFactor,
    bool  in_clearStencil,
    int32 in_clearStencilFactor) const
{
    if (auto buffer = GetDepthOnlyBuffer())
        buffer->Clear(in_clearDepth, in_clearDepthFactor, in_clearStencil, in_clearStencilFactor);
}

Ref<DepthBuffer> D11Renderer::GetDepthBuffer() const
{
    return m_swapChain->GetDepthBuffer();
}

Ref<DepthBuffer> D11Renderer::GetDepthOnlyBuffer() const
{
    return m_swapChain->GetDepthOnlyBuffer();
}

void D11Renderer::BindBackBufferMS(const Ref<DepthBuffer>& in_depthBuffer_or_null)
{
    if (auto rtv = GetBackBufferMS())
    {
        SetViewport(m_swapChain->GetViewport());
        SetRenderTarget(rtv->Get(),
                        in_depthBuffer_or_null ? in_depthBuffer_or_null->Get() : nullptr);
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
    }
}

void D11Renderer::ClearBackBufferMS(const Color& in_color)
{
    GetBackBufferMS()->Clear(in_color);
}

void D11Renderer::BeginGUI()
{
    ImguiVendor::BeginRender();
}

void D11Renderer::EndGUI()
{
    ImguiVendor::EndRender();
}

void D11Renderer::Present()
{
    m_swapChain->Present();
}

Ref<RenderTarget> D11Renderer::GetBackBufferMS() const
{
    return m_swapChain->GetBackBufferHDR();
}

ComPtr<ID3D11Device> D11Renderer::GetDevice() const
{
    return m_device;
}

ComPtr<ID3D11DeviceContext> D11Renderer::GetContext() const
{
    return m_deviceContext;
}

CommonState* D11Renderer::GetRenderStateStorage() const
{
    return m_renderStateStorage.get();
}

Ref<Swapchain> D11Renderer::GetSwapChain() const
{
    return m_swapChain;
}

Ref<RenderTarget> D11Renderer::GetBackBuffer() const
{
    return m_swapChain->GetBackBuffer();
}

Ref<RenderTarget> D11Renderer::GetBackBuffer()
{
    return m_swapChain->GetBackBuffer();
}

void D11Renderer::SetShaderResourceView(
    ID3D11ShaderResourceView* in_srv,
    uint32                    in_slot,
    eShaderFlags              in_bindFlags)
{
    if (BIT_AND(in_bindFlags, eShaderFlags_VertexShader))
    {
        if (m_vsTexture[in_slot] != in_srv)
        {
            m_vsTexture[in_slot] = in_srv;
            m_deviceContext->VSSetShaderResources(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_HullShader))
    {
        if (m_hsTexture[in_slot] != in_srv)
        {
            m_hsTexture[in_slot] = in_srv;
            m_deviceContext->HSSetShaderResources(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_DomainShader))
    {
        if (m_dsTexture[in_slot] != in_srv)
        {
            m_dsTexture[in_slot] = in_srv;
            m_deviceContext->DSSetShaderResources(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_GeometryShader))
    {
        if (m_gsTexture[in_slot] != in_srv)
        {
            m_gsTexture[in_slot] = in_srv;
            m_deviceContext->GSSetShaderResources(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_PixelShader))
    {
        if (m_psTexture[in_slot] != in_srv)
        {
            m_psTexture[in_slot] = in_srv;
            m_deviceContext->PSSetShaderResources(in_slot, 1, &in_srv);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_ComputeShader))
    {
        m_deviceContext->CSSetShaderResources(in_slot, 1, &in_srv);
        m_csBigestTextureBindedSlot = in_slot;
    }
}

void D11Renderer::SetConstantBuffer(
    ID3D11Buffer* in_constantBuffer,
    uint32        in_slot,
    eShaderFlags  in_bindFlags)
{
    if (BIT_AND(in_bindFlags, eShaderFlags_VertexShader))
    {
        if (m_vsBuffer[in_slot] != in_constantBuffer)
        {
            m_vsBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->VSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_HullShader))
    {
        if (m_hsBuffer[in_slot] != in_constantBuffer)
        {
            m_hsBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->HSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_DomainShader))
    {
        if (m_dsBuffer[in_slot] != in_constantBuffer)
        {
            m_dsBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->DSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_GeometryShader))
    {
        if (m_gsBuffer[in_slot] != in_constantBuffer)
        {
            m_gsBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->GSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_PixelShader))
    {
        if (m_psBuffer[in_slot] != in_constantBuffer)
        {
            m_psBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->PSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_bindFlags, eShaderFlags_ComputeShader))
    {
        if (m_csBuffer[in_slot] != in_constantBuffer)
        {
            m_csBuffer[in_slot] = in_constantBuffer;
            m_deviceContext->CSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }
}

void D11Renderer::SetUnorderedAccessView(ID3D11UnorderedAccessView* in_uav, uint32 in_slot) const
{
    m_deviceContext->CSSetUnorderedAccessViews(in_slot, 1, &in_uav, nullptr);
}

void D11Renderer::DispatchComputeShader(
    ID3D11ComputeShader* in_cs,
    uint32               in_threadGroupCountX,
    uint32               in_threadGroupCountY,
    uint32               in_threadGroupCountZ) const
{
    // dispatch
    m_deviceContext->CSSetShader(in_cs, nullptr, 0);
    m_deviceContext->Dispatch(in_threadGroupCountX, in_threadGroupCountY, in_threadGroupCountZ);

    // cs barrier
    static ID3D11UnorderedAccessView* nullUAVs[UAV_SLOT_MAX] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    m_deviceContext->CSSetUnorderedAccessViews(0, UAV_SLOT_MAX, nullUAVs, nullptr);

    static std::vector<ID3D11ShaderResourceView*> nullSRVs { TEXTURE_SLOT_MAX, nullptr };
    m_deviceContext->CSSetShaderResources(0, m_csBigestTextureBindedSlot + 1, nullSRVs.data());
}

}   // namespace crab
