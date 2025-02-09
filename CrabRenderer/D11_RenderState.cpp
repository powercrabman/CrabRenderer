#include "CrabPch.h"

#include "D11_RenderState.h"

#include "D11_Renderer.h"

namespace crab
{

//===================================================
// Depth Stencil State
//===================================================

Ref<D11_DepthStencilState> D11_DepthStencilState::Create(const D3D11_DEPTH_STENCIL_DESC& in_desc)
{
    Ref<D11_DepthStencilState> depthStencilState = CreateRef<D11_DepthStencilState>();
    auto                       d                 = D11_API->GetDevice();
    D11_ASSERT(d->CreateDepthStencilState(&in_desc, depthStencilState->m_depthStencil.GetAddressOf()),
                "CreateDepthStencilState Fail.");

    return depthStencilState;
}

Ref<D11_DepthStencilState> D11_DepthStencilState::Create(bool in_enableDepthTest, bool in_enableDepthWrite, bool in_enableStencilTest)
{
    // this factory is for simple usage.

    D3D11_DEPTH_STENCIL_DESC desc = {};

    // depth
    desc.DepthEnable    = in_enableDepthTest;
    desc.DepthWriteMask = in_enableDepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;

    // stencil
    desc.StencilEnable    = in_enableStencilTest;
    desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // if you want to concrete value, you can use another factory method.

    // Front face
    desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // Back face
    desc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    desc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    return Create(desc);
}

void D11_DepthStencilState::Bind()
{
    D11_API->SetDepthStencilState(m_depthStencil.Get());
}

//===================================================
// Rasterizer State
//===================================================

Ref<D11_RasterizerState> D11_RasterizerState::Create(const D3D11_RASTERIZER_DESC& in_desc)
{
    Ref<D11_RasterizerState> rasterizerState = CreateRef<D11_RasterizerState>();
    auto                     d               = D11_API->GetDevice();
    D11_ASSERT(d->CreateRasterizerState(&in_desc, rasterizerState->m_rasterizerState.GetAddressOf()),
                "CreateRasterizerState Fail.");

    return rasterizerState;
}

Ref<D11_RasterizerState> D11_RasterizerState::Create(eCullMode in_cullmode, eFillMode in_fillmode, eFrontFace in_frontface)
{
    D3D11_RASTERIZER_DESC desc = {};
    desc.FillMode              = static_cast<D3D11_FILL_MODE>(in_fillmode);
    desc.CullMode              = static_cast<D3D11_CULL_MODE>(in_cullmode);
    desc.FrontCounterClockwise = in_frontface == eFrontFace::CounterClockWise;
    desc.DepthBias             = 0;
    desc.DepthBiasClamp        = 0.0f;
    desc.SlopeScaledDepthBias  = 0.0f;
    desc.DepthClipEnable       = true;
    desc.ScissorEnable         = false;
    desc.MultisampleEnable     = false;   // not using MSAA
    desc.AntialiasedLineEnable = false;   // not using MSAA

    return Create(desc);
}

void D11_RasterizerState::Bind()
{
    D11_API->SetRasterizerState(m_rasterizerState.Get());
}

//===================================================
// Blend State
//===================================================

Ref<D11_BlendState> D11_BlendState::Create(const D3D11_BLEND_DESC& in_desc)
{
    Ref<D11_BlendState> blendState = CreateRef<D11_BlendState>();
    auto                d          = D11_API->GetDevice();
    D11_ASSERT(d->CreateBlendState(&in_desc, blendState->m_blendState.GetAddressOf()),
                "CreateBlendState Fail.");

    return blendState;
}

Ref<D11_BlendState> D11_BlendState::Create(bool in_alphaBlend)
{
    D3D11_BLEND_DESC desc       = {};
    desc.AlphaToCoverageEnable  = false;
    desc.IndependentBlendEnable = false;

    D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
    rtbd.BlendEnable                    = in_alphaBlend;

    if (in_alphaBlend)
    {
        rtbd.SrcBlend      = D3D11_BLEND_SRC_ALPHA;
        rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;

        rtbd.DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
        rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;

        rtbd.BlendOp      = D3D11_BLEND_OP_ADD;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    }
    else
    {
        rtbd.SrcBlend      = D3D11_BLEND_ONE;
        rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;

        rtbd.DestBlend      = D3D11_BLEND_ZERO;
        rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;

        rtbd.BlendOp      = D3D11_BLEND_OP_ADD;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    }

    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    desc.RenderTarget[0]       = rtbd; 

    return Create(desc);
}


void D11_BlendState::Bind()
{
    D11_API->SetBlendState(m_blendState.Get());
}

//===================================================
// Sampler State
//===================================================

Ref<D11_SamplerState> D11_SamplerState::Create(const D3D11_SAMPLER_DESC& in_desc)
{
    Ref<D11_SamplerState> samplerState = CreateRef<D11_SamplerState>();
    auto                  d            = D11_API->GetDevice();
    D11_ASSERT(d->CreateSamplerState(&in_desc, samplerState->m_samplerState.GetAddressOf()),
                "CreateSamplerState Fail.");

    return samplerState;
}

Ref<D11_SamplerState> D11_SamplerState::Create(eSamplerFilter in_filter, eSamplerAddress in_address)
{
    D3D11_SAMPLER_DESC desc = {};

    desc.Filter = static_cast<D3D11_FILTER>(in_filter);

    desc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_address);
    desc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_address);
    desc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(in_address);

    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    return Create(desc);
}

void D11_SamplerState::Bind(uint32 in_slot, eShaderFlags in_flags)
{
    D11_API->SetSamplerState(m_samplerState.Get(), in_slot, in_flags);
}

}   // namespace crab