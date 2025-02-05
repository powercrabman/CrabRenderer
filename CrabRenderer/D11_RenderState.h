#pragma once
#include "D11_Utiles.h"

namespace crab
{

//===================================================
// - Depth Stencil State
//===================================================

struct D11_DepthStencilState
{
    // - Factory
    static Ref<D11_DepthStencilState> Create(const D3D11_DEPTH_STENCIL_DESC& in_desc);
    static Ref<D11_DepthStencilState> Create(bool in_enableDepthTest,
                                             bool in_enableDepthWrite,
                                             bool in_enableStencilTest);

    ComPtr<ID3D11DepthStencilState> depthStencilState;
    D3D11_DEPTH_STENCIL_DESC        desc;
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
    ClockWise, // default
    CounterClockWise,
};

struct D11_RasterizerState
{
    // - Factory
    static Ref<D11_RasterizerState>             Create(const D3D11_RASTERIZER_DESC& in_desc);
    static crab::Ref<crab::D11_RasterizerState> Create(eCullMode  in_cullmode,
                                                       eFillMode  in_fillmode,
                                                       eFrontFace in_frontface);

    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_RASTERIZER_DESC         desc;
};

//===================================================
// - Blend State
//===================================================

struct D11_BlendState
{
    // - Factory
    static Ref<D11_BlendState> Create(const D3D11_BLEND_DESC& in_desc);
    static Ref<D11_BlendState> Create(bool in_alphaBlend);

    ComPtr<ID3D11BlendState> blendState;
    D3D11_BLEND_DESC         desc;
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

struct D11_SamplerState
{
    // - Factory
    static Ref<D11_SamplerState> Create(const D3D11_SAMPLER_DESC& in_desc);
    static Ref<D11_SamplerState> Create(eSamplerFilter in_filter, eSamplerAddress in_address);

    ComPtr<ID3D11SamplerState> samplerState;
    D3D11_SAMPLER_DESC         desc;
};

}   // namespace crab