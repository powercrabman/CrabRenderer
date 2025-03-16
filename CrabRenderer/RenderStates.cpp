#include "CrabPch.h"

#include "RenderStates.h"

#include "D11Renderer.h"

namespace crab
{

//===================================================
// Depth Stencil State
//===================================================

void DepthStencilState::Init(const D3D11_DEPTH_STENCIL_DESC& in_desc)
{
    auto d = GetRenderer().GetDevice();

    CheckD3D11Result(
        d->CreateDepthStencilState(
            &in_desc,
            m_depthStencil.GetAddressOf()),
        "CreateDepthStencilState Fail.");
}

void DepthStencilState::Bind(uint32 in_stencilRef) const
{
    GetRenderer().SetDepthStencilState(m_depthStencil.Get(), in_stencilRef);
}

//===================================================
// Rasterizer State
//===================================================

void RasterizerState::Init(const D3D11_RASTERIZER_DESC& in_desc)
{
    auto d = GetRenderer().GetDevice();
    CheckD3D11Result(
        d->CreateRasterizerState(
            &in_desc,
            m_rasterizerState.GetAddressOf()),
        "CreateRasterizerState Fail.");
}

void RasterizerState::Bind() const
{
    GetRenderer().SetRasterizerState(m_rasterizerState.Get());
}

void BlendState::Init(const D3D11_BLEND_DESC& in_desc)
{
    auto d = GetRenderer().GetDevice();
    CheckD3D11Result(
        d->CreateBlendState(
            &in_desc,
            m_blendState.GetAddressOf()),
        "CreateBlendState Fail.");
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

void SamplerState::Init(const D3D11_SAMPLER_DESC& in_desc)
{
    auto              d            = GetRenderer().GetDevice();
    CheckD3D11Result(
        d->CreateSamplerState(
            &in_desc,
            m_samplerState.GetAddressOf()),
        "CreateSamplerState Fail.");
}

void SamplerState::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetSamplerState(m_samplerState.Get(), in_slot, in_bindFlags);
}

}   // namespace crab