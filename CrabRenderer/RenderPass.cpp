#include "CrabPch.h"

#include "RenderPass.h"

#include "D11Renderer.h"
#include "RenderStates.h"
#include "Shaders.h"

namespace crab
{

Ref<RenderPass> RenderPass::Create(const Ref<VertexShader>& in_vertexShader, const Ref<PixelShader>& in_pixelShader, const Ref<GeometryShader>& in_geometryShader, const Ref<HullShader>& in_hullShader, const Ref<DomainShader>& in_domainShader, const Ref<DepthStencilState>& in_depthStencilState, const Ref<RasterizerState>& in_rasterizerState, const Ref<BlendState>& in_blendState, const SamplerList& in_commonSampler, const ConstantList& in_commonConstant)
{
    auto rp = CreateRef<RenderPass>();
    rp->SetVertexShader(in_vertexShader);
    rp->SetPixelShader(in_pixelShader);
    rp->SetGeometryShader(in_geometryShader);
    rp->SetHullShader(in_hullShader);
    rp->SetDomainShader(in_domainShader);
    rp->SetDepthStencilState(in_depthStencilState);
    rp->SetRasterizerState(in_rasterizerState);
    rp->SetBlendState(in_blendState);
    rp->SetSamplerStates(in_commonSampler);
    rp->SetConstantBuffers(in_commonConstant);
    return rp;
}

void RenderPass::SetVertexShader(const Ref<VertexShader>& in_shader)
{
    m_vertexShader = in_shader;
}

void RenderPass::SetPixelShader(const Ref<PixelShader>& in_shader)
{
    m_pixelShader = in_shader;
}

void RenderPass::SetGeometryShader(const Ref<GeometryShader>& in_shader)
{
    m_geometryShader = in_shader;
}

void RenderPass::SetHullShader(const Ref<HullShader>& in_shader)
{
    m_hullShader = in_shader;
}

void RenderPass::SetDomainShader(const Ref<DomainShader>& in_shader)
{
    m_domainShader = in_shader;
}

void RenderPass::SetDepthStencilState(const Ref<DepthStencilState>& in_state)
{
    m_depthStencilState = in_state;
}

void RenderPass::SetRasterizerState(const Ref<RasterizerState>& in_state)
{
    m_rasterizerState = in_state;
}

void RenderPass::SetBlendState(const Ref<BlendState>& in_state)
{
    m_blendState = in_state;
}

void RenderPass::SetSamplerStates(const SamplerList& in_states)
{
    m_samplerStates = in_states;
}

void RenderPass::SetConstantBuffers(const ConstantList& in_buffers)
{
    m_constantBuffers = in_buffers;
}

void RenderPass::BeginPass(uint32 in_stencilRef, const std::array<float, 4>& in_blendFactor) const
{
    auto& dx = GetRenderer();

    m_vertexShader->Bind();
    m_pixelShader->Bind();

    if (m_geometryShader)
        m_geometryShader->Bind();
    else
        dx.SetGeometryShader(nullptr);

    if (m_hullShader)
        m_hullShader->Bind();
    else
        dx.SetHullShader(nullptr);

    if (m_domainShader)
        m_domainShader->Bind();
    else
        dx.SetDomainShader(nullptr);

    m_depthStencilState->Bind(in_stencilRef);
    m_rasterizerState->Bind();
    m_blendState->Bind(in_blendFactor);
    m_samplerStates.Bind();
    m_constantBuffers.Bind();
}

}   // namespace crab