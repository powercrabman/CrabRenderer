#include "CrabPch.h"

#include "AppWindow.h"
#include "D11_Imgui.h"
#include "D11_Renderer.h"

#include "CrabEvent.h"
#include "D11_DepthBuffer.h"
#include "D11_RenderTarget.h"
#include "EventDispatcher.h"

#include "D11_Buffers.h"
#include "D11_ConstBuffer.h"
#include "D11_RenderState.h"
#include "D11_Shaders.h"
#include "D11_Swapchain.h"
#include "D11_Texture.h"

namespace crab
{

D11_Renderer::D11_Renderer()
{
}

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
    m_swapChain = D11_Swapchain::Create(in_setting.swapChainSetting);

#ifdef _DEBUG
    // - debug
    D11_ASSERT(m_device->QueryInterface(IID_PPV_ARGS(m_debug.GetAddressOf())), "QueryInterface Fail.");
    D11_ASSERT(m_debug->QueryInterface(IID_PPV_ARGS(m_infoQueue.GetAddressOf())), "QueryInterface Fail.");
#endif

    // imgui
    m_imgui = CreateScope<D11_Imgui>();
    m_imgui->Init();

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
                     m_swapChain->OnResize(e.width, e.height);
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
    D3D11_PRIMITIVE_TOPOLOGY topology = (D3D11_PRIMITIVE_TOPOLOGY)in_topology;
    if (m_topology != topology)
    {
        m_topology = topology;
        m_deviceContext->IASetPrimitiveTopology(topology);
    }
}

void D11_Renderer::SetVertexBuffer(ID3D11Buffer* in_vertexBuffer, uint32 in_stride, uint32 in_offset)
{
    if (m_vertexBuffer != in_vertexBuffer)
    {
        m_vertexBuffer = in_vertexBuffer;
        m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &in_stride, &in_offset);
    }
}

void D11_Renderer::SetIndexBuffer(ID3D11Buffer* in_indexBuffer)
{
    if (m_indexBuffer != in_indexBuffer)
    {
        m_indexBuffer = in_indexBuffer;
        m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }
}

void D11_Renderer::SetVertexShader(ID3D11VertexShader* in_shader, ID3D11InputLayout* in_layout)
{
    if (m_vertexShader != in_shader)
    {
        m_vertexShader = in_shader;
        m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
        m_deviceContext->IASetInputLayout(in_layout);
    }
}
void D11_Renderer::SetPixelShader(ID3D11PixelShader* in_shader)
{
    if (m_pixelShader != in_shader)
    {
        m_pixelShader = in_shader;
        m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
    }
}

void D11_Renderer::SetGeometryShader(ID3D11GeometryShader* in_shader)
{
    if (m_geometryShader != in_shader)
    {
        m_geometryShader = in_shader;
        m_deviceContext->GSSetShader(m_geometryShader, nullptr, 0);
    }
}

void D11_Renderer::SetComputeShader(ID3D11ComputeShader* in_shader)
{
    if (m_computeShader != in_shader)
    {
        m_computeShader = in_shader;
        m_deviceContext->CSSetShader(m_computeShader, nullptr, 0);
    }
}

void D11_Renderer::SetHullShader(ID3D11HullShader* in_shader)
{
    if (m_hullShader != in_shader)
    {
        m_hullShader = in_shader;
        m_deviceContext->HSSetShader(m_hullShader, nullptr, 0);
    }
}

void D11_Renderer::SetDomainShader(ID3D11DomainShader* in_shader)
{
    if (m_domainShader != in_shader)
    {
        m_domainShader = in_shader;
        m_deviceContext->DSSetShader(m_domainShader, nullptr, 0);
    }
}

void D11_Renderer::SetSamplerState(ID3D11SamplerState* in_samplerState, uint32 in_slot, eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        if (m_vsSamplerStates[in_slot] != in_samplerState)
        {

            m_vsSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->VSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        if (m_psSamplerStates[in_slot] != in_samplerState)
        {

            m_psSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->PSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_GeometryShader))
    {
        if (m_gsSamplerStates[in_slot] != in_samplerState)
        {

            m_gsSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->GSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_ComputeShader))
    {
        if (m_csSamplerStates[in_slot] != in_samplerState)
        {

            m_csSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->CSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_HullShader))
    {
        if (m_hsSamplerStates[in_slot] != in_samplerState)
        {
            m_hsSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->HSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_DomainShader))
    {
        if (m_dsSamplerStates[in_slot] != in_samplerState)
        {
            m_dsSamplerStates[in_slot] = in_samplerState;
            m_deviceContext->DSSetSamplers(in_slot, 1, &in_samplerState);
        }
    }
}

void D11_Renderer::SetRasterizerState(ID3D11RasterizerState* in_rasterizerState)
{
    if (m_rsState != in_rasterizerState)
    {
        m_rsState = in_rasterizerState;
        m_deviceContext->RSSetState(m_rsState);
    }
}

void D11_Renderer::SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState)
{
    if (m_dsState != in_depthStencilState)
    {
        m_dsState = in_depthStencilState;
        m_deviceContext->OMSetDepthStencilState(m_dsState, 0);
    }
}

void D11_Renderer::SetBlendState(ID3D11BlendState* in_blendState)
{
    if (m_bState != in_blendState)
    {
        m_bState = in_blendState;
        m_deviceContext->OMSetBlendState(m_bState, nullptr, 0xffffffff);
    }
}

void D11_Renderer::SetShaderResourceView(ID3D11ShaderResourceView* in_srv, uint32 in_slot, eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        if (m_vsTextures[in_slot] != in_srv)
        {
            m_vsTextures[in_slot] = in_srv;
            m_deviceContext->VSSetShaderResources(in_slot, 1, &in_srv);
            m_vsLargestBindedTextureSlot = in_slot;
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        if (m_psTextures[in_slot] != in_srv)
        {
            m_psTextures[in_slot] = in_srv;
            m_deviceContext->PSSetShaderResources(in_slot, 1, &in_srv);
            m_psLargestBindedTextureSlot = in_slot;
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_GeometryShader))
    {
        if (m_gsTextures[in_slot] != in_srv)
        {
            m_gsTextures[in_slot] = in_srv;
            m_deviceContext->GSSetShaderResources(in_slot, 1, &in_srv);
            m_gsLargestBindedTextureSlot = in_slot;
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_ComputeShader))
    {
        if (m_csTextures[in_slot] != in_srv)
        {
            m_csTextures[in_slot] = in_srv;
            m_deviceContext->CSSetShaderResources(in_slot, 1, &in_srv);
            m_csLargestBindedTextureSlot = in_slot;
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_HullShader))
    {
        if (m_hsTextures[in_slot] != in_srv)
        {
            m_hsTextures[in_slot] = in_srv;
            m_deviceContext->HSSetShaderResources(in_slot, 1, &in_srv);
            m_hsLargestBindedTextureSlot = in_slot;
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_DomainShader))
    {
        if (m_dsTextures[in_slot] != in_srv)
        {
            m_dsTextures[in_slot] = in_srv;
            m_deviceContext->DSSetShaderResources(in_slot, 1, &in_srv);
            m_dsLargestBindedTextureSlot = in_slot;
        }
    }
}

void D11_Renderer::ReleaseShaderResourceViews(eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        for (uint32 i = 0; i <= m_vsLargestBindedTextureSlot; ++i)
            m_vsTextures[i] = nullptr;
        m_deviceContext->VSSetShaderResources(0, m_vsLargestBindedTextureSlot + 1, m_vsTextures.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        for (uint32 i = 0; i <= m_psLargestBindedTextureSlot; ++i)
            m_psTextures[i] = nullptr;
        m_deviceContext->PSSetShaderResources(0, m_psLargestBindedTextureSlot + 1, m_vsTextures.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_GeometryShader))
    {
        for (uint32 i = 0; i <= m_gsLargestBindedTextureSlot; ++i)
            m_gsTextures[i] = nullptr;
        m_deviceContext->GSSetShaderResources(0, m_gsLargestBindedTextureSlot + 1, m_vsTextures.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_ComputeShader))
    {
        for (uint32 i = 0; i <= m_csLargestBindedTextureSlot; ++i)
            m_csTextures[i] = nullptr;
        m_deviceContext->CSSetShaderResources(0, m_csLargestBindedTextureSlot + 1, m_vsTextures.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_HullShader))
    {
        for (uint32 i = 0; i <= m_hsLargestBindedTextureSlot; ++i)
            m_hsTextures[i] = nullptr;
        m_deviceContext->HSSetShaderResources(0, m_hsLargestBindedTextureSlot + 1, m_vsTextures.data());
    }

    if (BIT_AND(in_flag, eShaderFlags_DomainShader))
    {
        for (uint32 i = 0; i <= m_dsLargestBindedTextureSlot; ++i)
            m_dsTextures[i] = nullptr;
        m_deviceContext->DSSetShaderResources(0, m_dsLargestBindedTextureSlot + 1, m_vsTextures.data());
    }
}

void D11_Renderer::SetConstantBuffer(ID3D11Buffer* in_constantBuffer, uint32 in_slot, eShaderFlags in_flag)
{
    if (BIT_AND(in_flag, eShaderFlags_VertexShader))
    {
        if (m_vsConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_vsConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->VSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_PixelShader))
    {
        if (m_psConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_psConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->PSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_GeometryShader))
    {
        if (m_gsConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_gsConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->GSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_ComputeShader))
    {
        if (m_csConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_csConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->CSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_HullShader))
    {
        if (m_hsConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_hsConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->HSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }

    if (BIT_AND(in_flag, eShaderFlags_DomainShader))
    {
        if (m_dsConstantBuffers[in_slot] != in_constantBuffer)
        {
            m_dsConstantBuffers[in_slot] = in_constantBuffer;
            m_deviceContext->DSSetConstantBuffers(in_slot, 1, &in_constantBuffer);
        }
    }
}

void D11_Renderer::SetRenderTarget(ID3D11RenderTargetView* in_renderTargetView, ID3D11DepthStencilView* in_depthStencilView)
{
    m_deviceContext->OMSetRenderTargets(1, &in_renderTargetView, in_depthStencilView);
}

void D11_Renderer::SetRenderTargets(const std::vector<ID3D11RenderTargetView*>& in_renderTargetViews, ID3D11DepthStencilView* in_depthStencilView)
{
    m_deviceContext->OMSetRenderTargets((UINT)in_renderTargetViews.size(), in_renderTargetViews.data(), in_depthStencilView);
}

void D11_Renderer::ReleaseRenderTargets()
{
    std::array<ID3D11RenderTargetView*, 8> nullRTVs = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    m_deviceContext->OMSetRenderTargets((UINT)nullRTVs.size(), nullRTVs.data(), nullptr);
}

void D11_Renderer::SetViewport(const Viewport& in_viewport)
{
    if (m_viewport != in_viewport)
    {
        m_viewport = in_viewport;
        m_deviceContext->RSSetViewports(1, in_viewport.Get11());
    }
}

void D11_Renderer::BindBackBuffer()
{
    auto rtv = m_swapChain->GetBackBufferRT();
    auto dsv = m_swapChain->GetDepthBuffer();
    SetViewport(m_swapChain->GetViewport());

    if (m_swapChain->HasFloatRenderTarget())
        SetRenderTarget(rtv->Get(), nullptr);
    else
        SetRenderTarget(rtv->Get(), dsv ? dsv->Get() : nullptr);
}

void D11_Renderer::ClearBackBuffer(const Color& in_color)
{
    m_swapChain->GetBackBufferRT()->Clear(in_color);
}

void D11_Renderer::ClearDepthBuffer(float in_depth, uint8 in_stencil) const
{
    if (auto buffer = GetDepthBuffer())
        buffer->Clear(in_depth, in_stencil);
}

Ref<D11_DepthBuffer> D11_Renderer::GetDepthBuffer() const
{
    return m_swapChain->GetDepthBuffer();
}

void D11_Renderer::BindFloatBackBuffer()
{
    if (auto rtv = GetFloatBackBuffer())
    {
        auto dsv = m_swapChain->GetDepthBuffer();
        SetViewport(m_swapChain->GetViewport());
        SetRenderTarget(rtv->Get(), dsv ? dsv->Get() : nullptr);
    }
    else
    {
        CRAB_DEBUG_BREAK("Not use Float Render Target.");
    }
}

void D11_Renderer::ClearFloatBackBuffer(const Color& in_color)
{
    GetFloatBackBuffer()->Clear(in_color);
}

void D11_Renderer::BeginGUI()
{
    m_imgui->BeginRender();
}

void D11_Renderer::EndGUI()
{
    m_imgui->EndRender();
}

void D11_Renderer::Present()
{
    m_swapChain->Present();
}

Ref<D11_RenderTarget> D11_Renderer::GetFloatBackBuffer() const
{
    return m_swapChain->GetFloatBackBufferRT();
}

Ref<D11_RenderTarget> D11_Renderer::GetFloatBackBuffer()
{
    return m_swapChain->GetFloatBackBufferRT();
}

ComPtr<ID3D11Device> D11_Renderer::GetDevice() const
{
    return m_device;
}

ComPtr<ID3D11DeviceContext> D11_Renderer::GetContext() const
{
    return m_deviceContext;
}

Ref<D11_Swapchain> D11_Renderer::GetSwapChain() const
{
    return m_swapChain;
}

Ref<D11_RenderTarget> D11_Renderer::GetBackBuffer() const
{
    return m_swapChain->GetBackBufferRT();
}

Ref<D11_RenderTarget> D11_Renderer::GetBackBuffer()
{
    return m_swapChain->GetBackBufferRT();
}

}   // namespace crab
