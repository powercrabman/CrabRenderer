#include "InternalPCH.h"

#include "DepthBuffer.h"

#include "RenderFactory.h"
#include "Renderer.h"
#include "ShaderResource.h"

namespace crab
{

void DepthBuffer::Init(uint32                in_width,
                       uint32                in_height,
                       eFormat               in_depthBufferFormat,
                       MSAA                  in_MSAA,
                       uint32                in_textureArraySize,
                       eTextureCreationFlags in_flags)
{
    ComPtr<ID3D11Texture2D> tex =
        d3d11::texture::CreateTexture2D(
            in_width,
            in_height,
            in_depthBufferFormat,
            D3D11_USAGE_DEFAULT,
            eBindFlags_DepthStencil,
            eCPUAccessFlags_None,
            in_MSAA,
            1,
            in_textureArraySize,
            in_flags,
            nullptr);

    Init_Internal(tex.Get(),
                  in_depthBufferFormat,
                  false,
                  eFormat::Unknown);
}

void DepthBuffer::InitForDepthMap(uint32                in_width,
                                         uint32                in_height,
                                         eFormat               in_depthBufferFormat,
                                         eFormat               in_shaderResourceFormat,
                                         MSAA                  in_MSAA,
                                         uint32                in_textureArraySize,
                                         eTextureCreationFlags in_flags)
{
    ComPtr<ID3D11Texture2D> tex =
        d3d11::texture::CreateTexture2D(
            in_width,
            in_height,
            eFormat::Typeless32_1,
            D3D11_USAGE_DEFAULT,
            eBindFlags_DepthStencil | eBindFlags_ShaderResource,
            eCPUAccessFlags_None,
            in_MSAA,
            1,
            in_textureArraySize,
            in_flags,
            nullptr);

    Init_Internal(tex.Get(),
                  in_depthBufferFormat,
                  true,
                  in_shaderResourceFormat);
}

void DepthBuffer::Clear(
    bool  in_clearDepth,
    float in_clearDepthFactor,
    bool  in_clearStencil,
    int32 in_clearStencilFactor) const
{
    auto dc = GetRenderer().GetDeviceContext();

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

void DepthBuffer::Init_Internal(ID3D11Texture2D* in_texture,
                                eFormat          in_depthBufferFormat,
                                bool             in_createShaderResource,
                                eFormat          in_shaderResourceFormat)
{
    D3D11_TEXTURE2D_DESC desc;
    in_texture->GetDesc(&desc);

    m_resolution = IVec2 { desc.Width, desc.Height };
    m_format     = in_depthBufferFormat;

    m_dsv = d3d11::views::CreateDSV(in_texture, in_depthBufferFormat);

    if (in_createShaderResource)
    {
        m_srv = factory::CreateTextureShaderResource(in_texture,
                                                     in_shaderResourceFormat);
    }
}

}   // namespace crab