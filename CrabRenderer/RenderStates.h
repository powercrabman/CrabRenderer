#pragma once
#include "RenderUtils.h"

namespace crab
{

//===================================================
// - Depth Stencil State
//===================================================

class DepthStencilState
{
public:
    // - Factory
    void Init(const D3D11_DEPTH_STENCIL_DESC& in_desc);
    void Bind(uint32 in_stencilRef = 0) const;

private:
    ComPtr<ID3D11DepthStencilState> m_depthStencil;
};

//===================================================
// - Rasterizer State
//===================================================

class RasterizerState
{
public:
    // - Factory
    void Init(const D3D11_RASTERIZER_DESC& in_desc);
    void Bind() const;

private:
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
};

//===================================================
// - Blend State
//===================================================

class BlendState
{
public:
    // - Factory
    void Init(const D3D11_BLEND_DESC& in_desc);
    void Bind(const std::array<float, 4>& in_blendFactors = { 1.f, 1.f, 1.f, 1.f }) const;

private:
    ComPtr<ID3D11BlendState> m_blendState;
};

//===================================================
// Sampler State
//===================================================

class SamplerState
{
public:
    // Factory
    void Init(const D3D11_SAMPLER_DESC& in_desc);
    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ComPtr<ID3D11SamplerState> Get() const { return m_samplerState.Get(); }

private:
    ComPtr<ID3D11SamplerState> m_samplerState;
};

}   // namespace crab