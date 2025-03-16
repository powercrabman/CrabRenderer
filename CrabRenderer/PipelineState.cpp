#include "CrabPch.h"

#include "PipelineState.h"

#include "D11Renderer.h"
#include "RenderStates.h"
#include "Shaders.h"

namespace crab
{

void PipelineState::Init(
    const Ref<VertexShader>&      in_vertexShader,
    const Ref<PixelShader>&       in_pixelShader,
    const Ref<GeometryShader>&    in_geometryShader,
    const Ref<HullShader>&        in_hullShader,
    const Ref<DomainShader>&      in_domainShader,
    const Ref<DepthStencilState>& in_depthStencilState,
    const Ref<RasterizerState>&   in_rasterizerState,
    const Ref<BlendState>&        in_blendState,
    const SamplerList&            in_samplers,
    const ConstantList&           in_buffers)
{
    m_vertexShader      = in_vertexShader;
    m_pixelShader       = in_pixelShader;
    m_geometryShader    = in_geometryShader;
    m_hullShader        = in_hullShader;
    m_domainShader      = in_domainShader;
    m_depthStencilState = in_depthStencilState;
    m_rasterizerState   = in_rasterizerState;
    m_blendState        = in_blendState;
    m_samplerStates     = in_samplers;
    m_constantBuffers   = in_buffers;
}

Ref<PipelineState> PipelineState::Clone() const
{
    Ref<PipelineState> result = CreateRef<PipelineState>();
    result->Init(
        m_vertexShader, 
        m_pixelShader, 
        m_geometryShader, 
        m_hullShader, 
        m_domainShader, 
        m_depthStencilState, 
        m_rasterizerState, 
        m_blendState, 
        m_samplerStates, 
        m_constantBuffers);
    return result;
}

void PipelineState::SetVertexShader(const Ref<VertexShader>& in_shader)
{
    m_vertexShader = in_shader;
}

void PipelineState::SetPixelShader(const Ref<PixelShader>& in_shader)
{
    m_pixelShader = in_shader;
}

void PipelineState::SetGeometryShader(const Ref<GeometryShader>& in_shader)
{
    m_geometryShader = in_shader;
}

void PipelineState::SetHullShader(const Ref<HullShader>& in_shader)
{
    m_hullShader = in_shader;
}

void PipelineState::SetDomainShader(const Ref<DomainShader>& in_shader)
{
    m_domainShader = in_shader;
}

void PipelineState::SetDepthStencilState(const Ref<DepthStencilState>& in_state)
{
    m_depthStencilState = in_state;
}

void PipelineState::SetRasterizerState(const Ref<RasterizerState>& in_state)
{
    m_rasterizerState = in_state;
}

void PipelineState::SetBlendState(const Ref<BlendState>& in_state)
{
    m_blendState = in_state;
}

void PipelineState::SetSamplers(const SamplerList& in_states)
{
    m_samplerStates = in_states;
}

void PipelineState::SetConstants(const ConstantList& in_buffers)
{
    m_constantBuffers = in_buffers;
}

void PipelineState::Bind(const PipelineBindArgument& in_args) const
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

    m_depthStencilState->Bind(in_args.stencilRef);
    m_rasterizerState->Bind();
    m_blendState->Bind(in_args.blendFactor);
    m_samplerStates.Bind();
    m_constantBuffers.Bind();
}

}   // namespace crab