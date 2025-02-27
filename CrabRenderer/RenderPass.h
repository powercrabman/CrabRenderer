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

class RenderPass
{
    D11_RESOURCE_CTOR(RenderPass);

public:
    static Ref<RenderPass> Create(
        const Ref<VertexShader>&      in_vertexShader,
        const Ref<PixelShader>&       in_pixelShader,
        const Ref<GeometryShader>&    in_geometryShader,
        const Ref<HullShader>&        in_hullShader,
        const Ref<DomainShader>&      in_domainShader,
        const Ref<DepthStencilState>& in_depthStencilState,
        const Ref<RasterizerState>&   in_rasterizerState,
        const Ref<BlendState>&        in_blendState,
        const SamplerList&       in_commonSampler,
        const ConstantList&     in_commonConstant);

    void SetVertexShader(const Ref<VertexShader>& in_shader);
    void SetPixelShader(const Ref<PixelShader>& in_shader);
    void SetGeometryShader(const Ref<GeometryShader>& in_shader);
    void SetHullShader(const Ref<HullShader>& in_shader);
    void SetDomainShader(const Ref<DomainShader>& in_shader);

    void SetDepthStencilState(const Ref<DepthStencilState>& in_state);
    void SetRasterizerState(const Ref<RasterizerState>& in_state);
    void SetBlendState(const Ref<BlendState>& in_state);
    void SetSamplerStates(const SamplerList& in_states);

    void SetConstantBuffers(const ConstantList& in_buffers);

    void BeginPass(uint32                      in_stencilRef  = 0,
                   const std::array<float, 4>& in_blendFactor = { 1.f, 1.f, 1.f, 1.f }) const;

private:
    Ref<VertexShader>   m_vertexShader;
    Ref<PixelShader>    m_pixelShader;
    Ref<GeometryShader> m_geometryShader;
    Ref<HullShader>     m_hullShader;
    Ref<DomainShader>   m_domainShader;

    Ref<DepthStencilState> m_depthStencilState;
    Ref<RasterizerState>   m_rasterizerState;
    Ref<BlendState>        m_blendState;
    SamplerList       m_samplerStates;

    ConstantList m_constantBuffers;
};

}   // namespace crab
