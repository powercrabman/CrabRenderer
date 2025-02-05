#pragma once

namespace crab
{

struct D11_Texture2D;

struct D11_DepthStencil
{
    // - Factory
    static Ref<D11_DepthStencil> Create(const Ref<D11_Texture2D>& in_tex, const D3D11_DEPTH_STENCIL_VIEW_DESC& in_desc);
    static Ref<D11_DepthStencil> CreateFromSwapChain(DXGI_FORMAT in_format);

    ComPtr<ID3D11DepthStencilView> dsv;
    D3D11_DEPTH_STENCIL_VIEW_DESC  desc;
};

}   // namespace crab