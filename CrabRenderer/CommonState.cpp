#include "CrabPch.h"

#include "CommonState.h"

#include "RenderStates.h"

namespace crab
{

void CommonState::Init()
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
    m_samplerPointWrap   = SamplerState::Create(samplerDesc);

    // Point Clamp
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_samplerPointClamp  = SamplerState::Create(samplerDesc);

    // Linear Wrap
    samplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    m_samplerLinearWrap  = SamplerState::Create(samplerDesc);

    // Linear Clamp
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_samplerLinearClamp = SamplerState::Create(samplerDesc);

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
    m_samplerShadowComparisonPointClamp = SamplerState::Create(samplerDesc);

    // Compriss Linear
    samplerDesc.Filter                   = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    m_samplerShadowComparisonLinearClamp = SamplerState::Create(samplerDesc);

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
    m_depthStencilDepthNone      = DepthStencilState::Create(depthStencilDesc);

    // Depth Test
    depthStencilDesc.DepthEnable = true;
    m_depthStencilDepthTest      = DepthStencilState::Create(depthStencilDesc);

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

    m_depthStencilDepthTestDrawStencil = DepthStencilState::Create(depthStencilDesc);

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

    m_depthStencilDepthTestDrawOnStencil = DepthStencilState::Create(depthStencilDesc);

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
    m_rasterizerCullNone                 = RasterizerState::Create(rasterizerDesc);

    // Cull None AA
    rasterizerDesc.CullMode              = D3D11_CULL_NONE;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.MultisampleEnable     = true;
    m_rasterizerCullNoneAA               = RasterizerState::Create(rasterizerDesc);

    // Cull Clockwise
    rasterizerDesc.CullMode              = D3D11_CULL_FRONT;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerCullClockwise            = RasterizerState::Create(rasterizerDesc);

    // Cull Clockwise AA
    rasterizerDesc.CullMode              = D3D11_CULL_FRONT;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerCullClockwiseAA          = RasterizerState::Create(rasterizerDesc);

    // Cull Counter Clockwise
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerCullCounterClockwise     = RasterizerState::Create(rasterizerDesc);

    // Cull Counter Clockwise AA
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerCullCounterClockwiseAA   = RasterizerState::Create(rasterizerDesc);

    // Rasterizer_Wireframe
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME;
    rasterizerDesc.MultisampleEnable     = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    m_rasterizerWireframe                = RasterizerState::Create(rasterizerDesc);

    // Rasterizer_Wireframe AA
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME;
    rasterizerDesc.AntialiasedLineEnable = true;
    rasterizerDesc.AntialiasedLineEnable = true;
    m_rasterizerWireframeAA              = RasterizerState::Create(rasterizerDesc);

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
    m_blendOpaque             = BlendState::Create(blendDesc);

    // Alpha Blend
    rtbd.BlendEnable               = true;
    rtbd.SrcBlend                  = D3D11_BLEND_BLEND_FACTOR;
    rtbd.DestBlend                 = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0]      = rtbd;
    m_blendAlphaBlendByBlendFactor = BlendState::Create(blendDesc);

    // Alpha Blend By Alpha
    rtbd.BlendEnable              = true;
    rtbd.SrcBlend                 = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend                = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0]     = rtbd;
    m_blendAlphaBlendByAlphaValue = BlendState::Create(blendDesc);

    // Anit Aliasing
    blendDesc.AlphaToCoverageEnable = true;

    // Blend_Opaque AA
    rtbd.BlendEnable          = false;
    blendDesc.RenderTarget[0] = rtbd;
    m_blendOpaqueAA           = BlendState::Create(blendDesc);

    // Alpha Blend AA
    rtbd.BlendEnable                 = true;
    rtbd.SrcBlend                    = D3D11_BLEND_BLEND_FACTOR;
    rtbd.DestBlend                   = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0]        = rtbd;
    m_blendAlphaBlendByBlendFactorAA = BlendState::Create(blendDesc);

    // Alpha Blend By Alpha AA
    rtbd.BlendEnable                = true;
    rtbd.SrcBlend                   = D3D11_BLEND_SRC_ALPHA;
    rtbd.DestBlend                  = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0]       = rtbd;
    m_blendAlphaBlendByAlphaValueAA = BlendState::Create(blendDesc);
}

Ref<SamplerState> CommonState::Sampler_PointWrap() const
{
    return m_samplerPointWrap;
}

Ref<SamplerState> CommonState::Sampler_PointClamp() const
{
    return m_samplerPointClamp;
}

Ref<SamplerState> CommonState::Sampler_LinearWrap() const
{
    return m_samplerLinearWrap;
}

Ref<SamplerState> CommonState::Sampler_LinearClamp() const
{
    return m_samplerLinearClamp;
}

Ref<SamplerState> CommonState::Sampler_ShadowComparisonLinearClamp() const
{
    return m_samplerShadowComparisonLinearClamp;
}

Ref<SamplerState> CommonState::Sampler_ShadowComparisonPointClamp() const
{
    return m_samplerShadowComparisonPointClamp;
}

Ref<DepthStencilState> CommonState::DepthStencil_DepthNone() const
{
    return m_depthStencilDepthNone;
}

Ref<DepthStencilState> CommonState::DepthStencil_DepthTest() const
{
    return m_depthStencilDepthTest;
}

Ref<DepthStencilState> CommonState::DepthStencil_DepthTestDrawStencil() const
{
    return m_depthStencilDepthTestDrawStencil;
}

Ref<DepthStencilState> CommonState::DepthStencil_DepthTestDrawOnStencil() const
{
    return m_depthStencilDepthTestDrawOnStencil;
}

Ref<RasterizerState> CommonState::Rasterizer_CullNone(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullNoneAA : m_rasterizerCullNone;
}

Ref<RasterizerState> CommonState::Rasterizer_CullClockwise(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullClockwiseAA : m_rasterizerCullClockwise;
}

Ref<RasterizerState> CommonState::Rasterizer_CullCounterClockwise(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerCullCounterClockwiseAA : m_rasterizerCullCounterClockwise;
}

Ref<RasterizerState> CommonState::Rasterizer_Wireframe(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_rasterizerWireframeAA : m_rasterizerWireframe;
}

Ref<BlendState> CommonState::Blend_Opaque(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendOpaqueAA : m_blendOpaque;
}

Ref<BlendState> CommonState::Blend_AlphaBlendByBlendFactor(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendAlphaBlendByBlendFactorAA : m_blendAlphaBlendByBlendFactor;
}

Ref<BlendState> CommonState::Blend_AlphaBlendByAlphaValue(bool in_antiAliasing) const
{
    return in_antiAliasing ? m_blendAlphaBlendByAlphaValueAA : m_blendAlphaBlendByAlphaValue;
}

}   // namespace crab