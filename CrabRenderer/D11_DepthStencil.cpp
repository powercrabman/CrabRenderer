#include "CrabPch.h"

#include "D11_DepthStencil.h"

#include "D11_Renderer.h"
#include "D11_Texture.h"

namespace crab
{

Ref<D11_DepthStencil> D11_DepthStencil::Create(const Ref<D11_Texture2D>& in_tex, const D3D11_DEPTH_STENCIL_VIEW_DESC& in_desc)
{
    Ref<D11_DepthStencil> depthStencil = CreateRef<D11_DepthStencil>();
    auto                  d            = D11_API->GetDevice();

    D11_ASSERT(d->CreateDepthStencilView(in_tex->texture.Get(), &in_desc, depthStencil->dsv.GetAddressOf()), "CreateDepthStencilView Fail.");

    depthStencil->desc = in_desc;

    return depthStencil;
}

crab::Ref<crab::D11_DepthStencil> D11_DepthStencil::CreateFromSwapChain(DXGI_FORMAT in_format)
{
    auto                  d            = D11_API->GetDevice();
    auto                  sc           = D11_API->GetSwapChain();
    Ref<D11_DepthStencil> depthStencil = CreateRef<D11_DepthStencil>();

    // Get Back buffer Texture
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    D11_ASSERT(sc->GetDesc(&scDesc), "GetDesc Fail.");

    // Create Depth Stencil Texture
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width                = scDesc.BufferDesc.Width;
    texDesc.Height               = scDesc.BufferDesc.Height;
    texDesc.MipLevels            = 1;
    texDesc.ArraySize            = 1;
    texDesc.Format               = in_format;
    texDesc.SampleDesc.Count     = 1;
    texDesc.SampleDesc.Quality   = 0;
    texDesc.Usage                = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;
    texDesc.CPUAccessFlags       = 0;
    texDesc.MiscFlags            = 0;

    Ref<D11_Texture2D> tex2D = D11_Texture::Create2D(texDesc);

    // Create Depth Stencil View
    D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
    desc.Format                        = in_format;
    desc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice            = 0;

    D11_ASSERT(d->CreateDepthStencilView(tex2D->texture.Get(), &desc, depthStencil->dsv.GetAddressOf()), "CreateDepthStencilView Fail.");

    // - Set Field
    depthStencil->desc = desc;
    return depthStencil;
}

}   // namespace crab