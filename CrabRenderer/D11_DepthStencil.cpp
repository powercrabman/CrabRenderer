#include "CrabPch.h"

#include "D11_DepthStencil.h"

#include "D11_Renderer.h"
#include "D11_Texture.h"
#include "AppWindow.h"

namespace crab
{

Ref<D11_DepthStencil> D11_DepthStencil::Create(DXGI_FORMAT in_format)
{
    auto                  d            = D11_API->GetDevice();
    auto                  sc           = D11_API->GetSwapChain();
    Ref<D11_DepthStencil> depthStencil = CreateRef<D11_DepthStencil>();

    auto [width, height] = GetApplication().GetAppWindow().GetWindowSize();

    // Create Depth Stencil Texture
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width                = UINT(width);
    texDesc.Height               = UINT(height);
    texDesc.MipLevels            = 1;
    texDesc.ArraySize            = 1;
    texDesc.Format               = in_format;
    texDesc.SampleDesc.Count     = 1;
    texDesc.SampleDesc.Quality   = 0;
    texDesc.Usage                = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;
    texDesc.CPUAccessFlags       = 0;
    texDesc.MiscFlags            = 0;

    ComPtr<ID3D11Texture2D> tex2D;
    D11_ASSERT(d->CreateTexture2D(&texDesc, nullptr, tex2D.GetAddressOf()), "CreateTexture2D Fail.");

    // Create Depth Stencil View
    D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
    desc.Format                        = in_format;
    desc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice            = 0;

    D11_ASSERT(d->CreateDepthStencilView(tex2D.Get(), &desc, depthStencil->m_dsv.GetAddressOf()), "CreateDepthStencilView Fail.");

    // - Set Field
    depthStencil->m_format = in_format;
    return depthStencil;
}

void D11_DepthStencil::Clear(bool in_clearDepth, bool in_clearStencil)
{
    auto dc = D11_API->GetContext();

    UINT clearFlags = 0;

    if (in_clearDepth)
        clearFlags |= D3D11_CLEAR_DEPTH;

    if (in_clearStencil)
        clearFlags |= D3D11_CLEAR_STENCIL;

    dc->ClearDepthStencilView(m_dsv.Get(), clearFlags, 1.f, 0);
}

}   // namespace crab