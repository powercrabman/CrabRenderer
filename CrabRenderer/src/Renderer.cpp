#include "InternalPCH.h"

#include "Renderer.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "SwapChain.h"

namespace crab
{

Ref<FrameBuffer> Renderer::GetBackBufferFB() const
{
    return m_swapChain->GetBackBufferFB();
}

Ref<FrameBuffer> Renderer::GetHDRBackBufferFB() const
{
    return m_swapChain->GetHDRBackBufferFB();
}

Ref<DepthBuffer> Renderer::GetDepthBuffer() const
{
    return m_swapChain->GetDepthBuffer();
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init(const RendererInitInfo& in_initInfo)
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
    d3d11::CheckOK(D3D11CreateDevice(nullptr,
                                     D3D_DRIVER_TYPE_HARDWARE,
                                     NULL,
                                     createDeviceFlags,
                                     featureLevels,
                                     ARRAYSIZE(featureLevels),
                                     D3D11_SDK_VERSION,
                                     m_device.GetAddressOf(),
                                     &m_config.featureLevel,
                                     m_deviceContext.GetAddressOf()),
                   "D3D11CreateDevice Fail.");

    m_config.hdr  = HDR { .enableHDR = in_initInfo.createHDR };
    m_config.msaa = MSAA::ComputeBestMSAA(in_initInfo.enableMSAA,
                                          m_device.Get(),
                                          d3d11::GetFormat(in_initInfo.backBufferFormat),
                                          MSAA_SAMPLE_COUNT);

    m_config.enableVSync = in_initInfo.vSync;

    // - Swap Chain
    m_swapChain = CreateRef<SwapChain>();
    m_swapChain->Init(in_initInfo);

#ifdef _DEBUG
    // - debug
    d3d11::CheckOK(m_device->QueryInterface(IID_PPV_ARGS(m_debug.GetAddressOf())), "QueryInterface Fail.");
    d3d11::CheckOK(m_debug->QueryInterface(IID_PPV_ARGS(m_infoQueue.GetAddressOf())), "QueryInterface Fail.");
#endif
}

void Renderer::InitGlobalResources(const std::filesystem::path& in_rendererDefaultShaderDirectory)
{
    GlobalShader::Init(in_rendererDefaultShaderDirectory);
    GlobalState::Init();
    GlobalConstants::Init();
}

void Renderer::Shutdown()
{
}

void Renderer::OnResize(uint32 in_width, uint32 in_height) const
{
    m_swapChain->OnResize(in_width, in_height);
}

void Renderer::Draw(uint32 in_vertexCount, uint32 in_startVertexLocation /*= 0*/) const
{
    m_deviceContext->Draw(in_vertexCount, in_startVertexLocation);
}

void Renderer::DrawIndexed(uint32 in_indexCount, uint32 in_startIndexLocation /*= 0*/, int32 in_baseVertexLocation /*= 0*/) const
{
    m_deviceContext->DrawIndexed(in_indexCount, in_startIndexLocation, in_baseVertexLocation);
}

void Renderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY in_topology)
{
    if (m_topology != in_topology)
    {
        m_topology = in_topology;
        m_deviceContext->IASetPrimitiveTopology(in_topology);
    }
}

void Renderer::SetVertexBuffer(ID3D11Buffer*  in_vertexBuffer,
                               uint32         in_stride,
                               uint32         in_offset,
                               VertexBufferID in_vertexBufferID)
{
    if (m_vertexBufferID != in_vertexBufferID || in_vertexBufferID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_vertexBufferID = in_vertexBufferID;
        m_deviceContext->IASetVertexBuffers(0, 1, &in_vertexBuffer, &in_stride, &in_offset);
    }
}

void Renderer::SetIndexBuffer(ID3D11Buffer* in_indexBuffer,
                              IndexBufferID in_indexBufferID)
{
    if (m_indexBufferID != in_indexBufferID || in_indexBufferID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_indexBufferID = in_indexBufferID;
        m_deviceContext->IASetIndexBuffer(in_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }
}

void Renderer::SetVertexShader(ID3D11VertexShader* in_shader,
                               VertexShaderID      in_vertexShaderID)
{
    if (m_vertexShaderID != in_vertexShaderID || in_vertexShaderID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_vertexShaderID = in_vertexShaderID;
        m_deviceContext->VSSetShader(in_shader, nullptr, 0);
    }
}

void Renderer::SetInputLayout(ID3D11InputLayout* in_inputLayout,
                              InputLayoutID      in_inputLayoutID)
{
    if (m_inputLayoutID != in_inputLayoutID || in_inputLayoutID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_inputLayoutID = in_inputLayoutID;
        m_deviceContext->IASetInputLayout(in_inputLayout);
    }
}

void Renderer::SetPixelShader(ID3D11PixelShader* in_shader,
                              PixelShaderID      in_pixelShaderID)
{
    if (m_pixelShaderID != in_pixelShaderID || in_pixelShaderID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_pixelShaderID = in_pixelShaderID;
        m_deviceContext->PSSetShader(in_shader, nullptr, 0);
    }
}

void Renderer::SetGeometryShader(ID3D11GeometryShader* in_shader,
                                 GeometryShaderID      in_geometryShaderID)
{
    if (m_geometryShaderID != in_geometryShaderID || in_geometryShaderID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_geometryShaderID = in_geometryShaderID;
        m_deviceContext->GSSetShader(in_shader, nullptr, 0);
    }
}

void Renderer::SetHullShader(ID3D11HullShader* in_shader,
                             HullShaderID      in_hullShaderID)
{
    if (m_hullShaderID != in_hullShaderID || in_hullShaderID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_hullShaderID = in_hullShaderID;
        m_deviceContext->HSSetShader(in_shader, nullptr, 0);
    }
}

void Renderer::SetDomainShader(ID3D11DomainShader* in_shader,
                               DomainShaderID      in_domainShaderID)
{
    if (m_domainShaderID != in_domainShaderID || in_domainShaderID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_domainShaderID = in_domainShaderID;
        m_deviceContext->DSSetShader(in_shader, nullptr, 0);
    }
}

void Renderer::SetSamplerState(
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

void Renderer::SetRasterizerState(ID3D11RasterizerState* in_rasterizerState,
                                  RasterizerStateID      in_rasterizerStateID)
{
    if (m_rasterizerStateID != in_rasterizerStateID || in_rasterizerStateID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_rasterizerStateID = in_rasterizerStateID;
        m_deviceContext->RSSetState(in_rasterizerState);
    }
}

void Renderer::SetDepthStencilState(ID3D11DepthStencilState* in_depthStencilState,
                                    uint32                   in_stencilRef,
                                    DepthStencilStateID      in_depthStencilStateID)
{
    if (m_depthStencilStateID != in_depthStencilStateID || m_stencilRef != in_stencilRef || in_depthStencilStateID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_depthStencilStateID = in_depthStencilStateID;
        m_stencilRef          = in_stencilRef;
        m_deviceContext->OMSetDepthStencilState(in_depthStencilState, in_stencilRef);
    }
}

void Renderer::SetBlendState(ID3D11BlendState*           in_blendState,
                             const std::array<float, 4>& in_blendFactor,
                             BlendStateID                in_blendStateID)
{
    if (m_blendStateID != in_blendStateID || m_blendFactor != in_blendFactor || in_blendStateID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_blendStateID = in_blendStateID;
        m_blendFactor  = in_blendFactor;
        m_deviceContext->OMSetBlendState(in_blendState, in_blendFactor.data(), 0xFFFFFFFF);
    }
}

void Renderer::SetViewport(const Viewport& in_viewport)
{
    if (m_viewport != in_viewport)
    {
        m_viewport = in_viewport;
        m_deviceContext->RSSetViewports(1, in_viewport.GetD3D11Viewport());
    }
}

void Renderer::ResetVertexBuffer()
{
    if (m_vertexBufferID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_vertexBufferID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
}

void Renderer::ResetIndexBuffer()
{
    if (m_indexBufferID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_indexBufferID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
}

void Renderer::ResetFrameBuffer()
{
    if (m_frameBufferID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_frameBufferID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
}

void Renderer::ResetVertexShader()
{
    if (m_vertexShaderID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_vertexShaderID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->VSSetShader(nullptr, nullptr, 0);
}

void Renderer::ResetPixelShader()
{
    if (m_pixelShaderID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_pixelShaderID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->PSSetShader(nullptr, nullptr, 0);
}

void Renderer::ResetGeometryShader()
{
    if (m_geometryShaderID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_geometryShaderID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->GSSetShader(nullptr, nullptr, 0);
}

void Renderer::ResetHullShader()
{
    if (m_hullShaderID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_hullShaderID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->HSSetShader(nullptr, nullptr, 0);
}

void Renderer::ResetDomainShader()
{
    if (m_domainShaderID == RENDERER_RESOURCE_NULL_ID)
        return;

    m_domainShaderID = RENDERER_RESOURCE_NULL_ID;
    m_deviceContext->DSSetShader(nullptr, nullptr, 0);
}

void Renderer::Present()
{
    m_swapChain->Present(m_config.enableVSync);
    m_frameBufferID = RENDERER_RESOURCE_NULL_ID;
}

ID3D11Device* Renderer::GetDevice() const
{
    return m_device.Get();
}

ID3D11DeviceContext* Renderer::GetDeviceContext() const
{
    return m_deviceContext.Get();
}

const Ref<SwapChain>& Renderer::GetSwapChain() const
{
    return m_swapChain;
}

void Renderer::SetShaderResourceView(
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

void Renderer::SetConstantBuffer(
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

void Renderer::SetUnorderedAccessView(ID3D11UnorderedAccessView* in_uav, uint32 in_slot) const
{
    m_deviceContext->CSSetUnorderedAccessViews(in_slot, 1, &in_uav, nullptr);
}

void Renderer::DispatchComputeShader(
    ID3D11ComputeShader* in_cs,
    uint32               in_threadGroupCountX,
    uint32               in_threadGroupCountY,
    uint32               in_threadGroupCountZ) const
{
    // dispatch
    m_deviceContext->CSSetShader(in_cs, nullptr, 0);
    m_deviceContext->Dispatch(in_threadGroupCountX, in_threadGroupCountY, in_threadGroupCountZ);

    // cs barrier
    static ID3D11UnorderedAccessView* nullUAVs[SHADER_UAV_SLOT_COUNT] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    m_deviceContext->CSSetUnorderedAccessViews(0, SHADER_UAV_SLOT_COUNT, nullUAVs, nullptr);

    static std::vector<ID3D11ShaderResourceView*> nullSRVs { SHADER_SRV_SLOT_COUNT, nullptr };
    m_deviceContext->CSSetShaderResources(0, m_csBigestTextureBindedSlot + 1, nullSRVs.data());
}

void Renderer::SetRenderTargets(ID3D11RenderTargetView* const* in_renderTargetViews,
                                uint32                         in_renderTargetCount,
                                ID3D11DepthStencilView*        in_depthStencilView,
                                FrameBufferID                  in_frameBufferID)
{
    if (m_frameBufferID != in_frameBufferID || in_frameBufferID == RENDERER_RESOURCE_UNKNOWN_ID)
    {
        m_frameBufferID = in_frameBufferID;
        m_deviceContext->OMSetRenderTargets(in_renderTargetCount,
                                            in_renderTargetViews,
                                            in_depthStencilView);
    }
}

}   // namespace crab
