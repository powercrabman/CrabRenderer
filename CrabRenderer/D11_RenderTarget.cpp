#include "CrabPch.h"

#include "D11_RenderTarget.h"

#include "D11_Texture.h"
#include "D11_Renderer.h"

namespace crab
{

Ref<D11_RenderTarget> D11_RenderTarget::Create(const Ref<D11_Texture2D>& in_texture)
{
    auto                  d  = D11_API->GetDevice();
    Ref<D11_RenderTarget> rt = CreateRef<D11_RenderTarget>();

    // - Create Render Target View
    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    desc.Format        = in_texture->desc.Format;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    D11_ASSERT(d->CreateRenderTargetView(in_texture->texture.Get(), &desc, rt->renderTargetView.GetAddressOf()), "CreateRenderTargetView Fail.");

    // - Set Field
    rt->desc = in_texture->desc;

    return rt;
}

crab::Ref<crab::D11_RenderTarget> D11_RenderTarget::CreateFromSwapChain()
{
    auto d  = D11_API->GetDevice();
    auto sc = D11_API->GetSwapChain();

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    D11_ASSERT(sc->GetDesc(&swapChainDesc), "GetDesc Fail.");

    Ref<D11_RenderTarget> rt = CreateRef<D11_RenderTarget>();

    // - Create Render Target View From Swap Chain
    ComPtr<ID3D11Texture2D> backBuffer;
    D11_ASSERT(sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf()), "GetBuffer Fail.");

    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format                        = swapChainDesc.BufferDesc.Format;
    desc.ViewDimension                 = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice            = 0;
    D11_ASSERT(d->CreateRenderTargetView(backBuffer.Get(), &desc, rt->renderTargetView.GetAddressOf()), "CreateRenderTargetView Fail.");

    // - Set Field
    D3D11_TEXTURE2D_DESC backBufferDesc;
    backBuffer->GetDesc(&backBufferDesc);

    rt->desc.Format = backBufferDesc.Format;

    return rt;
}

}   // namespace crab