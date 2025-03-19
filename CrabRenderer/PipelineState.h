#pragma once

#include "D11ResourceList.h"

namespace crab
{
class VertexShader;
class PixelShader;
class GeometryShader;
class HullShader;
class DomainShader;
class DepthStencilState;
class RasterizerState;
class BlendState;

//===================================================
// This class is imitation of D3D12 PSO
//===================================================

struct PipelineBindArgument
{
    PipelineBindArgument() = default;
    PipelineBindArgument(uint32 in_stencilRef, const std::array<float, 4>& in_blendFactor)
        : stencilRef(in_stencilRef)
        , blendFactor(in_blendFactor)
    {
    }
    PipelineBindArgument(uint32 in_stencilRef)
        : stencilRef(in_stencilRef)
        , blendFactor({ 1.0f, 1.0f, 1.0f, 1.0f })
    {
    }

    PipelineBindArgument(const std::array<float, 4>& in_blendFactor)
        : stencilRef(0)
        , blendFactor(in_blendFactor)
    {
    }

    uint32               stencilRef  = 0;
    std::array<float, 4> blendFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
};

class PipelineState
{
public:
    void Init(
        const Ref<VertexShader>&      in_vertexShader,
        const Ref<PixelShader>&       in_pixelShader,
        const Ref<GeometryShader>&    in_geometryShader,
        const Ref<HullShader>&        in_hullShader,
        const Ref<DomainShader>&      in_domainShader,
        const Ref<DepthStencilState>& in_depthStencilState,
        const Ref<RasterizerState>&   in_rasterizerState,
        const Ref<BlendState>&        in_blendState,
        const SamplerList&            in_samplers,
        const ConstantList&           in_buffers);

    Ref<PipelineState> Clone() const;

    void SetVertexShader(const Ref<VertexShader>& in_shader);
    void SetPixelShader(const Ref<PixelShader>& in_shader);
    void SetGeometryShader(const Ref<GeometryShader>& in_shader);
    void SetHullShader(const Ref<HullShader>& in_shader);
    void SetDomainShader(const Ref<DomainShader>& in_shader);

    void SetDepthStencilState(const Ref<DepthStencilState>& in_state);
    void SetRasterizerState(const Ref<RasterizerState>& in_state);
    void SetBlendState(const Ref<BlendState>& in_state);

    void SetSamplers(const SamplerList& in_states);
    void SetConstants(const ConstantList& in_buffers);

    void Bind(const PipelineBindArgument& in_args) const;

private:
    Ref<VertexShader>   m_vertexShader;
    Ref<PixelShader>    m_pixelShader;
    Ref<GeometryShader> m_geometryShader;
    Ref<HullShader>     m_hullShader;
    Ref<DomainShader>   m_domainShader;

    Ref<DepthStencilState> m_depthStencilState;
    Ref<RasterizerState>   m_rasterizerState;
    Ref<BlendState>        m_blendState;

    SamplerList  m_samplerStates;
    ConstantList m_constantBuffers;
};

}   // namespace crab
