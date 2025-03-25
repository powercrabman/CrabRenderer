#include "InternalPCH.h"

#include "RenderStates.h"

#include "D3D11Utils.h"
#include "Renderer.h"

namespace crab
{

//===================================================
// Depth Stencil State
//===================================================

void DepthStencilState::Init(
    bool             in_depthTestEnable,
    bool             in_depthWriteEnable,
    eComparisonFunc  in_depthFunc,
    bool             in_stencilEnable,
    eBlendWriteFlags in_stencilReadMask,
    eBlendWriteFlags in_stencilWriteMask,
    eComparisonFunc  in_frontFaceStencilFunc,
    eStencilOperator in_frontFaceDepthPassStencilFail,
    eStencilOperator in_frontFaceDepthFailStencilPass,
    eStencilOperator in_frontFaceDepthPassStencilPass,
    eComparisonFunc  in_backFaceStencilFunc,
    eStencilOperator in_backFaceDepthPassStencilFail,
    eStencilOperator in_backFaceDepthFailStencilPass,
    eStencilOperator in_backFaceDepthPassStencilPass)
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable              = in_depthTestEnable;
    desc.DepthWriteMask           = in_depthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc                = static_cast<D3D11_COMPARISON_FUNC>(in_depthFunc);

    desc.StencilEnable    = in_stencilEnable;
    desc.StencilReadMask  = in_stencilReadMask;
    desc.StencilWriteMask = in_stencilWriteMask;

    desc.FrontFace.StencilFunc        = static_cast<D3D11_COMPARISON_FUNC>(in_frontFaceStencilFunc);
    desc.FrontFace.StencilFailOp      = static_cast<D3D11_STENCIL_OP>(in_frontFaceDepthPassStencilFail);
    desc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(in_frontFaceDepthFailStencilPass);
    desc.FrontFace.StencilPassOp      = static_cast<D3D11_STENCIL_OP>(in_frontFaceDepthPassStencilPass);

    desc.BackFace.StencilFunc        = static_cast<D3D11_COMPARISON_FUNC>(in_backFaceStencilFunc);
    desc.BackFace.StencilFailOp      = static_cast<D3D11_STENCIL_OP>(in_backFaceDepthPassStencilFail);
    desc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(in_backFaceDepthFailStencilPass);
    desc.BackFace.StencilPassOp      = static_cast<D3D11_STENCIL_OP>(in_backFaceDepthPassStencilPass);

    auto d = GetRenderer().GetDevice();

    d3d11::CheckOK(d->CreateDepthStencilState(
                       &desc,
                       m_depthStencil.GetAddressOf()),
                   "CreateDepthStencilState Fail.");

    m_stateID = s_idGenerator.GenerateID();
}

void DepthStencilState::Bind(uint32 in_stencilRef) const
{
    GetRenderer().SetDepthStencilState(m_depthStencil.Get(), in_stencilRef, m_stateID);
}

//===================================================
// Rasterizer State
//===================================================

void RasterizerState::Init(
    eFillMode in_fillMode,
    eCullMode in_cullMode,
    int32     in_depthBias,
    float     in_depthBiasClamp,
    float     in_slopeScaledDepthBias,
    bool      in_depthClipEnable,
    bool      in_scissorEnable,
    bool      in_MSAAEnable)
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.FillMode              = static_cast<D3D11_FILL_MODE>(in_fillMode);
    desc.CullMode              = static_cast<D3D11_CULL_MODE>(in_cullMode);
    desc.FrontCounterClockwise = false;
    desc.DepthBias             = in_depthBias;
    desc.DepthBiasClamp        = in_depthBiasClamp;
    desc.SlopeScaledDepthBias  = in_slopeScaledDepthBias;
    desc.DepthClipEnable       = in_depthClipEnable;
    desc.ScissorEnable         = in_scissorEnable;
    desc.MultisampleEnable     = in_MSAAEnable;
    desc.AntialiasedLineEnable = in_MSAAEnable;

    auto d = GetRenderer().GetDevice();
    d3d11::CheckOK(
        d->CreateRasterizerState(
            &desc,
            m_rasterizerState.GetAddressOf()),
        "CreateRasterizerState Fail.");

    m_stateID = s_idGenerator.GenerateID();
}

void RasterizerState::Bind() const
{
    GetRenderer().SetRasterizerState(m_rasterizerState.Get(), m_stateID);
}

//===================================================
// Blend State
//===================================================

void BlendState::Init(
    bool             in_alphaToCoverageEnable,
    bool             in_blendEnable,
    eBlendFactor     in_srcBlendFactor,
    eBlendFactor     in_destBlendFactor,
    eBlendOperator   in_blendOperator,
    eBlendFactor     in_srcBlendAlphaFactor,
    eBlendFactor     in_destBlendAlphaFactor,
    eBlendOperator   in_blendAlphaOperator,
    eBlendWriteFlags in_renderTargetWriteMask)
{
    D3D11_BLEND_DESC desc       = {};
    desc.AlphaToCoverageEnable  = in_alphaToCoverageEnable;
    desc.IndependentBlendEnable = false;

    desc.RenderTarget[0].BlendEnable           = in_blendEnable;
    desc.RenderTarget[0].SrcBlend              = static_cast<D3D11_BLEND>(in_srcBlendFactor);
    desc.RenderTarget[0].DestBlend             = static_cast<D3D11_BLEND>(in_destBlendFactor);
    desc.RenderTarget[0].BlendOp               = static_cast<D3D11_BLEND_OP>(in_blendOperator);
    desc.RenderTarget[0].SrcBlendAlpha         = static_cast<D3D11_BLEND>(in_srcBlendAlphaFactor);
    desc.RenderTarget[0].DestBlendAlpha        = static_cast<D3D11_BLEND>(in_destBlendAlphaFactor);
    desc.RenderTarget[0].BlendOpAlpha          = static_cast<D3D11_BLEND_OP>(in_blendAlphaOperator);
    desc.RenderTarget[0].RenderTargetWriteMask = in_renderTargetWriteMask;

    auto d = GetRenderer().GetDevice();
    d3d11::CheckOK(
        d->CreateBlendState(
            &desc,
            m_blendState.GetAddressOf()),
        "CreateBlendState Fail.");

    m_stateID = s_idGenerator.GenerateID();
}

void BlendState::Bind(const std::array<float, 4>& in_blendFactors) const
{
    GetRenderer().SetBlendState(m_blendState.Get(), in_blendFactors, m_stateID);
}

//===================================================
// Sampler State
//===================================================

void SamplerState::Init(
    eSamplerMode        in_mode,
    eSamplerFilter      in_filter,
    eSamplerAddressMode in_addressU,
    eSamplerAddressMode in_addressV,
    eSamplerAddressMode in_addressW,
    float               in_mipLODBias,
    uint32              in_maxAnisotropy,
    eComparisonFunc     in_comparisonFunc,
    const Color4&       in_borderColor,
    float               in_minLOD,
    float               in_maxLOD)
{
    D3D11_SAMPLER_DESC desc;
    desc.Filter         = d3d11::GetSamplerFilter(in_mode, in_filter);
    desc.AddressU       = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_addressU);
    desc.AddressV       = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_addressV);
    desc.AddressW       = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_addressW);
    desc.MipLODBias     = in_mipLODBias;
    desc.MaxAnisotropy  = in_maxAnisotropy;
    desc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(in_comparisonFunc);
    desc.BorderColor[0] = in_borderColor.x;
    desc.BorderColor[1] = in_borderColor.y;
    desc.BorderColor[2] = in_borderColor.z;
    desc.BorderColor[3] = in_borderColor.w;
    desc.MinLOD         = in_minLOD;
    desc.MaxLOD         = in_maxLOD;

    d3d11::CheckOK(
        GetRenderer().GetDevice()->CreateSamplerState(
            &desc,
            m_samplerState.GetAddressOf()),
        "CreateSamplerState Fail.");
}

void SamplerState::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetSamplerState(m_samplerState.Get(), in_slot, in_bindFlags);
}

}   // namespace crab