#include "CrabPch.h"

#include "GlobalState.h"

#include "RenderStates.h"

namespace crab
{

void GlobalState::Init()
{
    //===================================================
    // Sampler State
    //===================================================

    D3D11_SAMPLER_DESC samplerDesc = {};
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.MipLODBias     = 0.f;
    samplerDesc.MaxAnisotropy  = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.BorderColor[0] = 0.f;
    samplerDesc.BorderColor[1] = 0.f;
    samplerDesc.BorderColor[2] = 0.f;
    samplerDesc.BorderColor[3] = 0.f;
    samplerDesc.MinLOD         = 0.f;
    samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

    // Point Wrap
    samplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    m_samplerPointWrap   = CreateRef<SamplerState>();
    m_samplerPointWrap->Init(samplerDesc);

    // Point Clamp
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_samplerPointClamp  = CreateRef<SamplerState>();
    m_samplerPointClamp->Init(samplerDesc);

    // Linear Wrap
    samplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    m_samplerLinearWrap  = CreateRef<SamplerState>();
    m_samplerLinearWrap->Init(samplerDesc);

    // Linear Clamp
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_samplerLinearClamp = CreateRef<SamplerState>();
    m_samplerLinearClamp->Init(samplerDesc);

    // Compriss Point
    samplerDesc.Filter                  = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU                = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV                = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW                = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.ComparisonFunc          = D3D11_COMPARISON_LESS_EQUAL;
    samplerDesc.BorderColor[0]          = 1000.f;
    samplerDesc.BorderColor[1]          = 1000.f;
    samplerDesc.BorderColor[2]          = 1000.f;
    samplerDesc.BorderColor[3]          = 1000.f;
    m_samplerShadowComparisonPointClamp = CreateRef<SamplerState>();
    m_samplerShadowComparisonPointClamp->Init(samplerDesc);

    // Compriss Linear
    samplerDesc.Filter                   = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    m_samplerShadowComparisonLinearClamp = CreateRef<SamplerState>();
    m_samplerShadowComparisonLinearClamp->Init(samplerDesc);

    //===================================================
    // Depth Stencil State
    //===================================================

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable    = false;
    depthStencilDesc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    // Depth None
    depthStencilDesc.DepthEnable = false;
    m_depthStencilDepthNone      = CreateRef<DepthStencilState>();
    m_depthStencilDepthNone->Init(depthStencilDesc);

    // Depth Test
    depthStencilDesc.DepthEnable = true;
    m_depthStencilDepthTest      = CreateRef<DepthStencilState>();
    m_depthStencilDepthTest->Init(depthStencilDesc);

    // Depth Test Draw Stencil
    depthStencilDesc.DepthEnable   = true;
    depthStencilDesc.StencilEnable = true;

    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_REPLACE;
    depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    m_depthStencilDepthTestDrawStencil = CreateRef<DepthStencilState>();
    m_depthStencilDepthTestDrawStencil->Init(depthStencilDesc);

    // Depth Test Draw On Stencil
    depthStencilDesc.DepthEnable   = true;
    depthStencilDesc.StencilEnable = true;

    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_EQUAL;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_EQUAL;
    depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    m_depthStencilDepthTestDrawOnStencil = CreateRef<DepthStencilState>();
    m_depthStencilDepthTestDrawOnStencil->Init(depthStencilDesc);

    //===================================================
    // Rasterizer State
    //===================================================

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.f;
    rasterizerDesc.SlopeScaledDepthBias  = 0.f;
    rasterizerDesc.DepthClipEnable       = true;
    rasterizerDesc.ScissorEnable         = false;

    // Cull None
    rasterizerDesc.CullMode              = D3D11_CULL_NONE;
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.MultisampleEnable     = false;
    m_rasterizerCullNone                 = CreateRef<RasterizerState>();
    m_rasterizerCullNone->Init(rasterizerDesc);

    // Cull None AA
    rasterizerDesc.CullMode              = D3D11_CULL_NONE;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.MultisampleEnable     = true;
    m_rasterizerCullNoneAA               = CreateRef<RasterizerState>();
    m_rasterizerCullNoneAA->Init(rasterizerDesc);

    // Cull Clockwise
    rasterizerDesc.CullMode              = D3D11_CULL_FRONT;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerCullClockwise            = CreateRef<RasterizerState>();
    m_rasterizerCullClockwise->Init(rasterizerDesc);

    // Cull Clockwise AA
    rasterizerDesc.CullMode              = D3D11_CULL_FRONT;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerCullClockwiseAA          = CreateRef<RasterizerState>();
    m_rasterizerCullClockwiseAA->Init(rasterizerDesc);

    // Cull Counter Clockwise
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerCullCounterClockwise     = CreateRef<RasterizerState>();
    m_rasterizerCullCounterClockwise->Init(rasterizerDesc);

    // Cull Counter Clockwise AA
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerCullCounterClockwiseAA   = CreateRef<RasterizerState>();
    m_rasterizerCullCounterClockwiseAA->Init(rasterizerDesc);

    // Rasterizer_Wireframe
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerWireframe                = CreateRef<RasterizerState>();
    m_rasterizerWireframe->Init(rasterizerDesc);

    // Rasterizer_Wireframe AA
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerWireframeAA              = CreateRef<RasterizerState>();
    m_rasterizerWireframeAA->Init(rasterizerDesc);

    //===================================================
    // Blend State
    //===================================================

    D3D11_BLEND_DESC blendDesc = {};
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.AlphaToCoverageEnable  = false;
    blendDesc.IndependentBlendEnable = false;

    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
    ZeroMemory(&rtbd, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
    rtbd.BlendEnable           = false;
    rtbd.SrcBlend              = D3D11_BLEND_ONE;
    rtbd.DestBlend             = D3D11_BLEND_ZERO;
    rtbd.BlendOp               = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha         = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha        = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Blend_Opaque
    rtbd.BlendEnable          = false;
    blendDesc.RenderTarget[0] = rtbd;
    m_blendOpaque             = CreateRef<BlendState>();
    m_blendOpaque->Init(blendDesc);

    // Alpha Blend
    rtbd.BlendEnable               = true;
    rtbd.SrcBlend                  = D3D11_BLEND_BLEND_FACTOR;
    rtbd.DestBlend                 = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0]      = rtbd;
    m_blendAlphaBlendByBlendFactor = CreateRef<BlendState>();
    m_blendAlphaBlendByBlendFactor->Init(blendDesc);

    // Alpha Blend By Alpha
    rtbd.BlendEnable              = true;
    rtbd.SrcBlend                 = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend                = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0]     = rtbd;
    m_blendAlphaBlendByAlphaValue = CreateRef<BlendState>();
    m_blendAlphaBlendByAlphaValue->Init(blendDesc);

    // Anit Aliasing
    blendDesc.AlphaToCoverageEnable = true;

    // Blend_Opaque AA
    rtbd.BlendEnable          = false;
    blendDesc.RenderTarget[0] = rtbd;
    m_blendOpaqueAA           = CreateRef<BlendState>();
    m_blendOpaqueAA->Init(blendDesc);

    // Alpha Blend AA
    rtbd.BlendEnable                 = true;
    rtbd.SrcBlend                    = D3D11_BLEND_BLEND_FACTOR;
    rtbd.DestBlend                   = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0]        = rtbd;
    m_blendAlphaBlendByBlendFactorAA = CreateRef<BlendState>();
    m_blendAlphaBlendByBlendFactorAA->Init(blendDesc);

    // Alpha Blend By Alpha AA
    rtbd.BlendEnable                = true;
    rtbd.SrcBlend                   = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend                  = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0]       = rtbd;
    m_blendAlphaBlendByAlphaValueAA = CreateRef<BlendState>();
    m_blendAlphaBlendByAlphaValueAA->Init(blendDesc);
}

Ref<SamplerState> GlobalState::Sampler_PointWrap() const
{
    return m_samplerPointWrap;
}

Ref<SamplerState> GlobalState::Sampler_PointClamp() const
{
    return m_samplerPointClamp;
}

Ref<SamplerState> GlobalState::Sampler_LinearWrap() const
{
    return m_samplerLinearWrap;
}

Ref<SamplerState> GlobalState::Sampler_LinearClamp() const
{
    return m_samplerLinearClamp;
}

Ref<SamplerState> GlobalState::Sampler_ShadowComparisonLinearClamp() const
{
    return m_samplerShadowComparisonLinearClamp;
}

Ref<SamplerState> GlobalState::Sampler_ShadowComparisonPointClamp() const
{
    return m_samplerShadowComparisonPointClamp;
}

Ref<DepthStencilState> GlobalState::DepthStencil_DepthNone() const
{
    return m_depthStencilDepthNone;
}

Ref<DepthStencilState> GlobalState::DepthStencil_DepthTest() const
{
    return m_depthStencilDepthTest;
}

Ref<DepthStencilState> GlobalState::DepthStencil_DepthTestDrawStencil() const
{
    return m_depthStencilDepthTestDrawStencil;
}

Ref<DepthStencilState> GlobalState::DepthStencil_DepthTestDrawOnStencil() const
{
    return m_depthStencilDepthTestDrawOnStencil;
}

Ref<RasterizerState> GlobalState::Rasterizer_CullNone(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullNoneAA : m_rasterizerCullNone;
}

Ref<RasterizerState> GlobalState::Rasterizer_CullFront(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullClockwiseAA : m_rasterizerCullClockwise;
}

Ref<RasterizerState> GlobalState::Rasterizer_CullBack(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullCounterClockwiseAA : m_rasterizerCullCounterClockwise;
}

Ref<RasterizerState> GlobalState::Rasterizer_Wireframe(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerWireframeAA : m_rasterizerWireframe;
}

Ref<BlendState> GlobalState::Blend_Opaque(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendOpaqueAA : m_blendOpaque;
}

Ref<BlendState> GlobalState::Blend_AlphaBlendByBlendFactor(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendAlphaBlendByBlendFactorAA : m_blendAlphaBlendByBlendFactor;
}

Ref<BlendState> GlobalState::Blend_AlphaBlendByAlphaValue(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendAlphaBlendByAlphaValueAA : m_blendAlphaBlendByAlphaValue;
}

}   // namespace crab