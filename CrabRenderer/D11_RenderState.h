#pragma once
#include "D11_Utiles.h"

namespace crab
{

//===================================================
// - Depth Stencil State
//===================================================

class D11_DepthStencilState
{
public:
    // - Factory
    static Ref<D11_DepthStencilState> Create(const D3D11_DEPTH_STENCIL_DESC& in_desc);
    static Ref<D11_DepthStencilState> Create(bool in_enableDepthTest,
                                             bool in_enableDepthWrite,
                                             bool in_enableStencilTest);

    void Bind();

private:
    ComPtr<ID3D11DepthStencilState> m_depthStencil;
};

//===================================================
// - Rasterizer State
//===================================================

enum class eFillMode
{
    Solid     = D3D11_FILL_SOLID,
    Wireframe = D3D11_FILL_WIREFRAME
};

enum class eCullMode
{
    None  = D3D11_CULL_NONE,
    Front = D3D11_CULL_FRONT,
    Back  = D3D11_CULL_BACK
};

enum class eFrontFace
{
    ClockWise,   // default
    CounterClockWise,
};

class D11_RasterizerState
{
public:
    // - Factory
    static Ref<D11_RasterizerState>             Create(const D3D11_RASTERIZER_DESC& in_desc);
    static crab::Ref<crab::D11_RasterizerState> Create(eCullMode  in_cullmode,
                                                       eFillMode  in_fillmode,
                                                       eFrontFace in_frontface);

    void Bind();

private:
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
};

//===================================================
// - Blend State
//===================================================

class D11_BlendState
{
public:
    // - Factory
    static Ref<D11_BlendState> Create(const D3D11_BLEND_DESC& in_desc);
    static Ref<D11_BlendState> Create(bool in_alphaBlend);

    void Bind();

private:
    ComPtr<ID3D11BlendState> m_blendState;
};

//===================================================
// - Sampler State
//===================================================

enum class eSamplerFilter
{
    Point       = D3D11_FILTER_MIN_MAG_MIP_POINT,
    Linear      = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
    Anisotropic = D3D11_FILTER_ANISOTROPIC
};

enum class eSamplerAddress
{
    Wrap       = D3D11_TEXTURE_ADDRESS_WRAP,
    Clamp      = D3D11_TEXTURE_ADDRESS_CLAMP,
    Border     = D3D11_TEXTURE_ADDRESS_BORDER,
    Mirror     = D3D11_TEXTURE_ADDRESS_MIRROR,
    MirrorOnce = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

class D11_SamplerState
{
public:
    // - Factory
    static Ref<D11_SamplerState> Create(const D3D11_SAMPLER_DESC& in_desc);
    static Ref<D11_SamplerState> Create(eSamplerFilter in_filter, eSamplerAddress in_address);

    ID3D11SamplerState* Get() const { return m_samplerState.Get(); }
    void                Bind(uint32 in_slot, eShaderFlags in_flags);

private:
    ComPtr<ID3D11SamplerState> m_samplerState;
};

class D11_SamplerStateList
{
public:
    void ClearList();
    void Bind();

    D11_SamplerStateList& Add(Ref<D11_SamplerState> in_sampler, uint32 in_slot, eShaderFlags in_flags);
    Ref<D11_SamplerState> GetSampler(uint32 in_index) const;

private:
    struct SamplerNode
    {
        Ref<D11_SamplerState> sampler;
        uint32                slot;
        eShaderFlags          flags;
    };
    std::vector<SamplerNode> m_nodes;
};

}   // namespace crab