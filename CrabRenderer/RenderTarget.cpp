#include "CrabPch.h"

#include "RenderTarget.h"

#include "D11Renderer.h"
#include "DepthBuffer.h"
#include "RenderStates.h"
#include "Textures.h"

namespace crab
{

Ref<RenderTarget> RenderTarget::Create(ID3D11Texture2D* in_texture)
{
    Ref<RenderTarget> rt = CreateRef<RenderTarget>();

    D3D11_TEXTURE2D_DESC texDesc = {};
    in_texture->GetDesc(&texDesc);

    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format                        = texDesc.Format;
    desc.Texture2D.MipSlice            = 0;
    desc.ViewDimension                 = texDesc.SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

    CheckD3D11Result(
        GetRenderer().GetDevice()->CreateRenderTargetView(in_texture,
                                                          &desc,
                                                          rt->m_renderTargetView.GetAddressOf()),
        "CreateRenderTargetView Fail.");

    rt->m_image  = Image2D::CreateFromTexture(in_texture);
    rt->m_format = static_cast<eFormat>(texDesc.Format);

    return rt;
}

Ref<RenderTarget> RenderTarget::Create(
    uint32  in_width,
    uint32  in_height,
    eFormat in_format,
    uint32  in_MSAASampleCount,
    uint32  in_MSAAQuality)
{
    Ref<RenderTarget> rt = CreateRef<RenderTarget>();

    auto tex = ID3D11Texture2DUtil::CreateDefaultTexture2D(
        in_width,
        in_height,
        in_format,
        in_MSAASampleCount,
        in_MSAAQuality,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
        0);

    return Create(tex.Get());
}

void RenderTarget::Bind() const
{
    GetRenderer().SetRenderTarget(m_renderTargetView.Get(), nullptr);
}

void RenderTarget::Bind(const Ref<DepthBuffer>& in_depthBuffer) const
{
    GetRenderer().SetRenderTarget(m_renderTargetView.Get(), in_depthBuffer->Get());
}

void RenderTarget::Clear(const Color& in_color) const
{
    GetRenderer().GetContext()->ClearRenderTargetView(m_renderTargetView.Get(), (FLOAT*)&in_color);
}

}   // namespace crab