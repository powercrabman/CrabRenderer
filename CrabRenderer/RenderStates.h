#pragma once
#include "D11Utils.h"

namespace crab
{

//===================================================
// - Depth Stencil State
//===================================================

class DepthStencilState
{
    D11_RESOURCE_CTOR(DepthStencilState);

public:
    // - Factory
    static Ref<DepthStencilState> Create(const D3D11_DEPTH_STENCIL_DESC& in_desc);

    void Bind(uint32 in_stencilRef = 0) const;

private:
    ComPtr<ID3D11DepthStencilState> m_depthStencil;
};

//===================================================
// - Rasterizer State
//===================================================

class RasterizerState
{
    D11_RESOURCE_CTOR(RasterizerState);

public:
    // - Factory
    static Ref<RasterizerState> Create(const D3D11_RASTERIZER_DESC& in_desc);

    void Bind() const;

private:
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
};

//===================================================
// - Blend State
//===================================================

class BlendState
{
    D11_RESOURCE_CTOR(BlendState);

public:
    // - Factory
    static Ref<BlendState> Create(const D3D11_BLEND_DESC& in_desc);

    void Bind(const std::array<float, 4>& in_blendFactors = {1.f,1.f,1.f,1.f}) const;

private:
    ComPtr<ID3D11BlendState> m_blendState;
};

//===================================================
// Sampler State
//===================================================

class SamplerState
{
    D11_RESOURCE_CTOR(SamplerState);

public:
    // Factory
    static Ref<SamplerState> Create(const D3D11_SAMPLER_DESC& in_desc);

    ComPtr<ID3D11SamplerState> Get() const { return m_samplerState.Get(); }
    void                       Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

private:
    ComPtr<ID3D11SamplerState> m_samplerState;
};

}   // namespace crab