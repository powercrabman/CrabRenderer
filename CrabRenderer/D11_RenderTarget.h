#pragma once
namespace crab
{

struct D11_Texture2D;

struct D11_RenderTarget
{
    // - Factory
    static Ref<D11_RenderTarget> Create(const Ref<D11_Texture2D>& in_texture);
    static Ref<D11_RenderTarget> CreateFromSwapChain();

    ComPtr<ID3D11RenderTargetView> renderTargetView;
    D3D11_TEXTURE2D_DESC           desc;
};

}   // namespace crab