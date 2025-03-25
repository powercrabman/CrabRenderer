#include "InternalPCH.h"

#include "RenderTarget.h"

#include "D3D11Utils.h"
#include "DepthBuffer.h"
#include "RenderStates.h"
#include "Renderer.h"
#include "ShaderResource.h"

namespace crab
{

void RenderTarget::Init(ID3D11Texture2D* in_texture)
{
    D3D11_TEXTURE2D_DESC texDesc = {};
    in_texture->GetDesc(&texDesc);

    D3D11_RENDER_TARGET_VIEW_DESC desc = {};
    desc.Format                        = texDesc.Format;
    desc.Texture2D.MipSlice            = 0;
    desc.ViewDimension                 = texDesc.SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

    d3d11::CheckOK(
        GetRenderer().GetDevice()->CreateRenderTargetView(in_texture,
                                                          &desc,
                                                          m_renderTargetView.GetAddressOf()),
        "CreateRenderTargetView Fail.");

    m_image = CreateRef<Texture2D>();
    m_image->Init(in_texture, static_cast<eFormat>(texDesc.Format));
    m_format     = static_cast<eFormat>(texDesc.Format);
    m_resolution = IVec2 { texDesc.Width, texDesc.Height };
}

void RenderTarget::Init(
    uint32     in_width,
    uint32     in_height,
    eFormat    in_format,
    eBindFlags in_bindFlags,
    MSAA       in_MSAA)
{
    auto tex = d3d11::texture::CreateTexture2D(
        in_width,
        in_height,
        in_format,
        D3D11_USAGE_DEFAULT,
        in_bindFlags,
        eCPUAccessFlags_None,
        in_MSAA,
        1,
        1,
        eTextureCreationFlags_None,
        nullptr);

    Init(tex.Get());
}

void RenderTarget::Bind(const Ref<DepthBuffer>& in_depthBuffer)
{
    GetRenderer().SetRenderTargets(m_renderTargetView.GetAddressOf(),
                                   1,
                                   in_depthBuffer ?
                                       in_depthBuffer->GetDSV() :
                                       nullptr);
}

void RenderTarget::Clear(const Color4& in_color) const
{
    GetRenderer().GetDeviceContext()->ClearRenderTargetView(m_renderTargetView.Get(),
                                                            (float*)&in_color);
}

}   // namespace crab