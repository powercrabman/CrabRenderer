#pragma once
namespace crab
{

class SamplerState;
class DepthStencilState;
class RasterizerState;
class BlendState;

class CommonState
{
public:
    void Init();

    //===================================================
    // Sampler State
    //===================================================

    Ref<SamplerState> Sampler_PointWrap() const;
    Ref<SamplerState> Sampler_PointClamp() const;
    Ref<SamplerState> Sampler_LinearWrap() const;
    Ref<SamplerState> Sampler_LinearClamp() const;

    Ref<SamplerState> Sampler_ShadowComparisonLinearClamp() const;
    Ref<SamplerState> Sampler_ShadowComparisonPointClamp() const;

    //===================================================
    // Depth Stencil State
    //===================================================

    Ref<DepthStencilState> DepthStencil_DepthNone() const;
    Ref<DepthStencilState> DepthStencil_DepthTest() const;
    Ref<DepthStencilState> DepthStencil_DepthTestDrawStencil() const;
    Ref<DepthStencilState> DepthStencil_DepthTestDrawOnStencil() const;

    //===================================================
    // Rasterizer State
    //===================================================

    Ref<RasterizerState> Rasterizer_CullNone(bool in_antiAliasing) const;
    Ref<RasterizerState> Rasterizer_CullClockwise(bool in_antiAliasing) const;
    Ref<RasterizerState> Rasterizer_CullCounterClockwise(bool in_antiAliasing) const;
    Ref<RasterizerState> Rasterizer_Wireframe(bool in_antiAliasing) const;

    //===================================================
    // Blend State
    //===================================================

    Ref<BlendState> Blend_Opaque(bool in_antiAliasing) const;
    Ref<BlendState> Blend_AlphaBlendByBlendFactor(bool in_antiAliasing) const;
    Ref<BlendState> Blend_AlphaBlendByAlphaValue(bool in_antiAliasing) const;

private:
    // Sampler State
    Ref<SamplerState> m_samplerPointWrap;
    Ref<SamplerState> m_samplerPointClamp;
    Ref<SamplerState> m_samplerLinearWrap;
    Ref<SamplerState> m_samplerLinearClamp;

    Ref<SamplerState> m_samplerShadowComparisonLinearClamp;
    Ref<SamplerState> m_samplerShadowComparisonPointClamp;

    // Depth Stencil State
    Ref<DepthStencilState> m_depthStencilDepthNone;
    Ref<DepthStencilState> m_depthStencilDepthTest;
    Ref<DepthStencilState> m_depthStencilDepthTestDrawStencil;
    Ref<DepthStencilState> m_depthStencilDepthTestDrawOnStencil;

    // Rasterizer State
    Ref<RasterizerState> m_rasterizerCullNone;
    Ref<RasterizerState> m_rasterizerCullNoneAA;
    Ref<RasterizerState> m_rasterizerCullClockwise;
    Ref<RasterizerState> m_rasterizerCullClockwiseAA;
    Ref<RasterizerState> m_rasterizerCullCounterClockwise;
    Ref<RasterizerState> m_rasterizerCullCounterClockwiseAA;
    Ref<RasterizerState> m_rasterizerWireframe;
    Ref<RasterizerState> m_rasterizerWireframeAA;

    // Blend State
    Ref<BlendState> m_blendOpaque;
    Ref<BlendState> m_blendOpaqueAA;
    Ref<BlendState> m_blendAlphaBlendByBlendFactor;
    Ref<BlendState> m_blendAlphaBlendByBlendFactorAA;
    Ref<BlendState> m_blendAlphaBlendByAlphaValue;
    Ref<BlendState> m_blendAlphaBlendByAlphaValueAA;
};

}   // namespace crab
