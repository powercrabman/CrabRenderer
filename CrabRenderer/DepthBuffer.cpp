#include "CrabPch.h"

#include "DepthBuffer.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "Textures.h"

namespace crab
{

Ref<DepthBuffer> DepthBuffer::Create(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthBufferFormat,
    uint32  in_MSAASampleCount,
    uint32  in_MSAAQuality)
{
    Ref<DepthBuffer> db = CreateRef<DepthBuffer>();

    ComPtr<ID3D11Texture2D> depthBufferTexture = ID3D11Texture2DUtil::CreateDefaultTexture2D(
        in_width,
        in_height,
        in_depthBufferFormat,
        eBindFlags_DepthStencil,
        in_MSAASampleCount,
        in_MSAAQuality);

    db->m_format = in_depthBufferFormat;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format                        = static_cast<DXGI_FORMAT>(in_depthBufferFormat);
    dsvDesc.ViewDimension                 = in_MSAASampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice            = 0;

    CheckD3D11Result(
        GetRenderer().GetDevice()->CreateDepthStencilView(
            depthBufferTexture.Get(),
            &dsvDesc,
            db->m_dsv.GetAddressOf()),
        "CreateDepthStencilView Fail.");

    return db;
}

Ref<DepthBuffer> DepthBuffer::CreateWithImage2D(
    uint32  in_width,
    uint32  in_height,
    eFormat in_depthBufferFormat,
    uint32  in_MSAASampleCount,
    uint32  in_MSAAQuality,
    eFormat in_imageFormat)
{
    Ref<DepthBuffer> db = CreateRef<DepthBuffer>();

    ComPtr<ID3D11Texture2D> depthBufferTexture = ID3D11Texture2DUtil::CreateDefaultTexture2D(
        in_width,
        in_height,
        eFormat::Float32_Typeless,
        eBindFlags_DepthStencil | eBindFlags_ShaderResource,
        in_MSAASampleCount,
        in_MSAAQuality);

    db->m_format = in_depthBufferFormat;
    db->m_image  = Image2D::CreateFromTexture(depthBufferTexture.Get(), in_imageFormat);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format                        = static_cast<DXGI_FORMAT>(in_depthBufferFormat);
    dsvDesc.ViewDimension                 = in_MSAASampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice            = 0;

    CheckD3D11Result(
        GetRenderer().GetDevice()->CreateDepthStencilView(
            depthBufferTexture.Get(),
            &dsvDesc,
            db->m_dsv.GetAddressOf()),
        "CreateDepthStencilView Fail.");

    return db;
}

void DepthBuffer::Clear(
    bool  in_clearDepth,
    float in_clearDepthFactor,
    bool  in_clearStencil,
    int32 in_clearStencilFactor) const
{
    auto dc = GetRenderer().GetContext();

    UINT clearFlags = 0;

    if (in_clearDepth)
        clearFlags |= D3D11_CLEAR_DEPTH;

    if (in_clearStencil)
        clearFlags |= D3D11_CLEAR_STENCIL;

    dc->ClearDepthStencilView(
        m_dsv.Get(),
        clearFlags,
        in_clearDepthFactor,
        in_clearStencilFactor);
}

}   // namespace crab