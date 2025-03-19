#include "CrabPch.h"

#include "DepthBuffer.h"

#include "AppWindow.h"
#include "D11Renderer.h"
#include "Textures.h"

namespace crab
{

void DepthBuffer::Init(uint32  in_width,
                       uint32  in_height,
                       eFormat in_depthBufferFormat,
                       MSAA    in_MSAA)
{

    ComPtr<ID3D11Texture2D> depthBufferTexture =
        ID3D11Texture2DUtil::CreateTexture2D(in_width,
                                             in_height,
                                             eFormat::Typeless32_1,
                                             D3D11_USAGE_DEFAULT,
                                             eBindFlags_DepthStencil,
                                             eCPUAccessFlags_None,
                                             in_MSAA,
                                             1,
                                             1,
                                             eTextureCreationFlags_None,
                                             nullptr);

    m_format     = in_depthBufferFormat;
    m_resolution = { static_cast<int32>(in_width), static_cast<int32>(in_height) };
    m_dsv        = ID3D11DepthStencilViewUtil::CreateDepthStencilView(depthBufferTexture.Get(), in_depthBufferFormat);
}

void DepthBuffer::Init(ID3D11Texture2D* in_texture,
                       eFormat          in_depthBufferFormat)
{
    D3D11_TEXTURE2D_DESC desc;
    in_texture->GetDesc(&desc);

    m_dsv        = ID3D11DepthStencilViewUtil::CreateDepthStencilView(in_texture, in_depthBufferFormat);
    m_format     = in_depthBufferFormat;
    m_resolution = { static_cast<int32>(desc.Width), static_cast<int32>(desc.Height) };
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