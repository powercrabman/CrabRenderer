#include "CrabPch.h"

#include "RenderStates.h"

#include "D11Renderer.h"

namespace crab
{

//===================================================
// Depth Stencil State
//===================================================

Ref<DepthStencilState> DepthStencilState::Create(const D3D11_DEPTH_STENCIL_DESC& in_desc)
{
    Ref<DepthStencilState> depthStencilState = CreateRef<DepthStencilState>();
    auto                   d                 = GetRenderer().GetDevice();

    CheckD3D11Result(
        d->CreateDepthStencilState(&in_desc,
                                   depthStencilState->m_depthStencil.GetAddressOf()),
        "CreateDepthStencilState Fail.");

    return depthStencilState;
}

void DepthStencilState::Bind(uint32 in_stencilRef) const
{
    GetRenderer().SetDepthStencilState(m_depthStencil.Get(), in_stencilRef);
}

//===================================================
// Rasterizer State
//===================================================

Ref<RasterizerState> RasterizerState::Create(const D3D11_RASTERIZER_DESC& in_desc)
{
    Ref<RasterizerState> rasterizerState = CreateRef<RasterizerState>();
    auto                 d               = GetRenderer().GetDevice();
    CheckD3D11Result(d->CreateRasterizerState(&in_desc, rasterizerState->m_rasterizerState.GetAddressOf()),
                   "CreateRasterizerState Fail.");

    return rasterizerState;
}

void RasterizerState::Bind() const
{
    GetRenderer().SetRasterizerState(m_rasterizerState.Get());
}

Ref<BlendState> BlendState::Create(const D3D11_BLEND_DESC& in_desc)
{
    Ref<BlendState> blendState = CreateRef<BlendState>();
    auto            d          = GetRenderer().GetDevice();
    CheckD3D11Result(d->CreateBlendState(&in_desc, blendState->m_blendState.GetAddressOf()),
                   "CreateBlendState Fail.");

    return blendState;
}

//===================================================
// Blend State
//===================================================

void BlendState::Bind(const std::array<float, 4>& in_blendFactors) const
{
    GetRenderer().SetBlendState(m_blendState.Get(), in_blendFactors);
}

//===================================================
// Sampler State
//===================================================

Ref<SamplerState> SamplerState::Create(const D3D11_SAMPLER_DESC& in_desc)
{
    Ref<SamplerState> samplerState = CreateRef<SamplerState>();
    auto              d            = GetRenderer().GetDevice();
    CheckD3D11Result(d->CreateSamplerState(&in_desc, samplerState->m_samplerState.GetAddressOf()),
                   "CreateSamplerState Fail.");

    return samplerState;
}

void SamplerState::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetSamplerState(m_samplerState.Get(), in_slot, in_bindFlags);
}

}   // namespace crab