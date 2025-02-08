#include "CrabPch.h"

#include "D11_RenderTarget.h"

#include "D11_Renderer.h"
#include "D11_Texture.h"
#include "D11_RenderState.h"

namespace crab
{

Ref<D11_RenderTarget> D11_RenderTarget::Create(ID3D11Texture2D* in_texture)
{
    auto d = D11_API->GetDevice();

    Ref<D11_RenderTarget> rt = CreateRef<D11_RenderTarget>();

    D3D11_TEXTURE2D_DESC texDesc = {};
    in_texture->GetDesc(&texDesc);

    // - Create Render Target View
    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format                        = texDesc.Format;
    desc.ViewDimension                 = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice            = 0;

    D11_ASSERT(d->CreateRenderTargetView(in_texture,
                                         &desc,
                                         rt->m_renderTargetView.GetAddressOf()),
               "CreateRenderTargetView Fail.");

    if (BIT_AND(texDesc.BindFlags, D3D11_BIND_SHADER_RESOURCE))
    {
        // - Create Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format                          = texDesc.Format;
        srvDesc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip       = 0;
        srvDesc.Texture2D.MipLevels             = 1;

        rt->m_texture = D11_Texture::Create(
            in_texture,
            srvDesc,
            D11_SamplerState::Create(eSamplerFilter::Linear, eSamplerAddress::Clamp));
    }
    rt->m_format = texDesc.Format;

    return rt;
}

void D11_RenderTarget::Clear(const Color& in_color)
{
    D11_API->GetContext()->ClearRenderTargetView(m_renderTargetView.Get(), (FLOAT*)&in_color);
}

}   // namespace crab