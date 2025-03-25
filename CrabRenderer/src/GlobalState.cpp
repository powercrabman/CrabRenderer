#include "InternalPCH.h"


#include "GlobalState.h"

#include "RenderFactory.h"
#include "RenderStates.h"

namespace crab
{

struct GlobalState::Impl
{
    // Sampler State
    Ref<SamplerState> samplerPointWrap;
    Ref<SamplerState> samplerPointClamp;
    Ref<SamplerState> samplerLinearWrap;
    Ref<SamplerState> samplerLinearClamp;

    Ref<SamplerState> samplerComparisonLinearWrap;
    Ref<SamplerState> samplerComparisonLinearClamp;

    // Depth Stencil State
    Ref<DepthStencilState> depthStencilNoDepthNoStencil;
    Ref<DepthStencilState> depthStencilDepthNoStencil;
    Ref<DepthStencilState> depthStencilDepthWriteStencil;
    Ref<DepthStencilState> depthStencilDepthDrawOnStencil;

    // Rasterizer State
    Ref<RasterizerState> rasterizerSolidCullNone;
    Ref<RasterizerState> rasterizerSolidCullFront;
    Ref<RasterizerState> rasterizerSolidCullBack;

    Ref<RasterizerState> rasterizerWireframeCullNone;
    Ref<RasterizerState> rasterizerWireframeCullFront;
    Ref<RasterizerState> rasterizerWireframeCullBack;

    Ref<RasterizerState> rasterizerSolidCullNoneMSAA;
    Ref<RasterizerState> rasterizerSolidCullFrontMSAA;
    Ref<RasterizerState> rasterizerSolidCullBackMSAA;

    Ref<RasterizerState> rasterizerWireframeCullNoneMSAA;
    Ref<RasterizerState> rasterizerWireframeCullFrontMSAA;
    Ref<RasterizerState> rasterizerWireframeCullBackMSAA;

    // Blend State
    Ref<BlendState> blendNoBlend;
    Ref<BlendState> blendBlendBlendFactor;
    Ref<BlendState> blendBlendAlphaValue;
    Ref<BlendState> blendAddBlendFactor;
    Ref<BlendState> blendAddAlphaValue;

    Ref<BlendState> blendNoBlendMSAA;
    Ref<BlendState> blendBlendBlendFactorMSAA;
    Ref<BlendState> blendBlendAlphaValueMSAA;
    Ref<BlendState> blendAddBlendFactorMSAA;
    Ref<BlendState> blendAddAlphaValueMSAA;
};

Scope<GlobalState::Impl> GlobalState::s_impl = nullptr;

//===================================================

void GlobalState::Init()
{
    s_impl = CreateScope<Impl>();

    // Sampler State

    auto CreateSamplerState = [](
                                  eSamplerMode        in_mode,
                                  eSamplerFilter      in_filter,
                                  eSamplerAddressMode in_addressMode)
    {
        return factory::CreateSamplerState(
            in_mode,
            in_filter,
            in_addressMode,
            1,
            eComparisonFunc::LessEqual,
            color4::BLACK);
    };

    //===================================================
    // Sampler
    //===================================================

    s_impl->samplerPointWrap = CreateSamplerState(
        eSamplerMode::Default,
        eSamplerFilter::MinMagMipPoint,
        eSamplerAddressMode::Wrap);

    s_impl->samplerPointClamp = CreateSamplerState(
        eSamplerMode::Default,
        eSamplerFilter::MinMagMipPoint,
        eSamplerAddressMode::Clamp);

    s_impl->samplerLinearWrap = CreateSamplerState(
        eSamplerMode::Default,
        eSamplerFilter::MinMagMipLinear,
        eSamplerAddressMode::Wrap);

    s_impl->samplerLinearClamp = CreateSamplerState(
        eSamplerMode::Default,
        eSamplerFilter::MinMagMipLinear,
        eSamplerAddressMode::Clamp);

    s_impl->samplerComparisonLinearWrap = CreateSamplerState(
        eSamplerMode::Comparison,
        eSamplerFilter::MinMagLinear_MipPoint,
        eSamplerAddressMode::Wrap);

    s_impl->samplerComparisonLinearClamp = CreateSamplerState(
        eSamplerMode::Comparison,
        eSamplerFilter::MinMagLinear_MipPoint,
        eSamplerAddressMode::Clamp);

    //===================================================
    // Depth Stencil
    //===================================================

    s_impl->depthStencilNoDepthNoStencil = factory::CreateDepthStencilState(
        false,
        false,
        eComparisonFunc::AlwaysPass,
        false,
        eStencilOperator::Replace,
        eStencilOperator::Replace,
        eStencilOperator::Replace,
        eComparisonFunc::AlwaysPass);

    s_impl->depthStencilDepthNoStencil = factory::CreateDepthStencilState(
        true,
        true,
        eComparisonFunc::LessEqual,
        false,
        eStencilOperator::Replace,
        eStencilOperator::Replace,
        eStencilOperator::Replace,
        eComparisonFunc::AlwaysPass);

    s_impl->depthStencilDepthWriteStencil = factory::CreateDepthStencilState(
        true,
        true,
        eComparisonFunc::LessEqual,
        true,
        eStencilOperator::Keep,
        eStencilOperator::Keep,
        eStencilOperator::Replace,
        eComparisonFunc::AlwaysPass);

    s_impl->depthStencilDepthDrawOnStencil = factory::CreateDepthStencilState(
        true,
        true,
        eComparisonFunc::LessEqual,
        true,
        eStencilOperator::Keep,
        eStencilOperator::Keep,
        eStencilOperator::Keep,
        eComparisonFunc::Equal);

    //===================================================
    // Rasterizer State
    //===================================================

    s_impl->rasterizerSolidCullNone = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::None,
        false);

    s_impl->rasterizerSolidCullFront = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::Front,
        false);

    s_impl->rasterizerSolidCullBack = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::Back,
        false);

    s_impl->rasterizerWireframeCullNone = factory::CreateRasterizerState(

        eFillMode::Wireframe,
        eCullMode::None,
        false);

    s_impl->rasterizerWireframeCullFront = factory::CreateRasterizerState(
        eFillMode::Wireframe,
        eCullMode::Front,
        false);

    s_impl->rasterizerWireframeCullBack = factory::CreateRasterizerState(
        eFillMode::Wireframe,
        eCullMode::Back,
        false);

    s_impl->rasterizerSolidCullNoneMSAA = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::None,
        true);

    s_impl->rasterizerSolidCullFrontMSAA = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::Front,
        true);

    s_impl->rasterizerSolidCullBackMSAA = factory::CreateRasterizerState(
        eFillMode::Solid,
        eCullMode::Back,
        true);

    s_impl->rasterizerWireframeCullNoneMSAA = factory::CreateRasterizerState(
        eFillMode::Wireframe,
        eCullMode::None,
        true);

    s_impl->rasterizerWireframeCullFrontMSAA = factory::CreateRasterizerState(
        eFillMode::Wireframe,
        eCullMode::Front,
        true);
    s_impl->rasterizerWireframeCullBackMSAA = factory::CreateRasterizerState(
        eFillMode::Wireframe,
        eCullMode::Back,
        true);
    //===================================================
    // Blend State
    //===================================================

    s_impl->blendNoBlend = factory::CreateBlendState(
        false,
        false,
        eBlendFactor::One,
        eBlendFactor::Zero,
        eBlendOperator::Add);

    s_impl->blendBlendBlendFactor = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::InvSrcAlpha,
        eBlendOperator::Add);

    s_impl->blendBlendAlphaValue = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::InvSrcAlpha,
        eBlendOperator::Add);

    s_impl->blendAddBlendFactor = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::One,
        eBlendOperator::Add);

    s_impl->blendAddAlphaValue = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::One,
        eBlendOperator::Add);

    s_impl->blendNoBlendMSAA = factory::CreateBlendState(
        false,
        false,
        eBlendFactor::One,
        eBlendFactor::Zero,
        eBlendOperator::Add);

    s_impl->blendBlendBlendFactorMSAA = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::InvSrcAlpha,
        eBlendOperator::Add);

    s_impl->blendBlendAlphaValueMSAA = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::InvSrcAlpha,
        eBlendOperator::Add);

    s_impl->blendAddBlendFactorMSAA = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::One,
        eBlendOperator::Add);

    s_impl->blendAddAlphaValueMSAA = factory::CreateBlendState(
        false,
        true,
        eBlendFactor::SrcAlpha,
        eBlendFactor::One,
        eBlendOperator::Add);
}

Ref<SamplerState> GlobalState::Sampler_Point_Wrap()
{
    return s_impl->samplerPointWrap;
}

Ref<SamplerState> GlobalState::Sampler_Point_Clamp()
{
    return s_impl->samplerPointClamp;
}

Ref<SamplerState> GlobalState::Sampler_Linear_Wrap()
{
    return s_impl->samplerLinearWrap;
}

Ref<SamplerState> GlobalState::Sampler_Linear_Clamp()
{
    return s_impl->samplerLinearClamp;
}

Ref<SamplerState> GlobalState::Sampler_Comparison_Linear_Wrap()
{
    return s_impl->samplerComparisonLinearWrap;
}

Ref<SamplerState> GlobalState::Sampler_Comparison_Linear_Clamp()
{
    return s_impl->samplerComparisonLinearClamp;
}

Ref<DepthStencilState> GlobalState::DepthStencil_NoDepth_NoStencil()
{
    return s_impl->depthStencilNoDepthNoStencil;
}

Ref<DepthStencilState> GlobalState::DepthStencil_Depth_NoStencil()
{
    return s_impl->depthStencilDepthNoStencil;
}

Ref<DepthStencilState> GlobalState::DepthStencil_Depth_WriteStencil()
{
    return s_impl->depthStencilDepthWriteStencil;
}

Ref<DepthStencilState> GlobalState::DepthStencil_Depth_DrawOnStencil()
{
    return s_impl->depthStencilDepthDrawOnStencil;
}

Ref<RasterizerState> GlobalState::Rasterizer_Solid_CullNone(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerSolidCullNoneMSAA : s_impl->rasterizerSolidCullNone;
}

Ref<RasterizerState> GlobalState::Rasterizer_Solid_CullFront(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerSolidCullFrontMSAA : s_impl->rasterizerSolidCullFront;
}

Ref<RasterizerState> GlobalState::Rasterizer_Solid_CullBack(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerSolidCullBackMSAA : s_impl->rasterizerSolidCullBack;
}

Ref<RasterizerState> GlobalState::Rasterizer_Wireframe_CullNone(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerWireframeCullNoneMSAA : s_impl->rasterizerWireframeCullNone;
}

Ref<RasterizerState> GlobalState::Rasterizer_Wireframe_CullFront(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerWireframeCullFrontMSAA : s_impl->rasterizerWireframeCullFront;
}

Ref<RasterizerState> GlobalState::Rasterizer_Wireframe_CullBack(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->rasterizerWireframeCullBackMSAA : s_impl->rasterizerWireframeCullBack;
}

Ref<BlendState> GlobalState::Blend_NoBlend(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->blendNoBlendMSAA : s_impl->blendNoBlend;
}

Ref<BlendState> GlobalState::Blend_Blend_BlendFactor(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->blendBlendBlendFactorMSAA : s_impl->blendBlendBlendFactor;
}

Ref<BlendState> GlobalState::Blend_Blend_AlphaValue(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->blendBlendAlphaValueMSAA : s_impl->blendBlendAlphaValue;
}

Ref<BlendState> GlobalState::Blend_Add_BlendFactor(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->blendAddBlendFactorMSAA : s_impl->blendAddBlendFactor;
}

Ref<BlendState> GlobalState::Blend_Add_AlphaValue(bool in_MSAAEnable)
{
    return in_MSAAEnable ? s_impl->blendAddAlphaValueMSAA : s_impl->blendAddAlphaValue;
}

}   // namespace crab