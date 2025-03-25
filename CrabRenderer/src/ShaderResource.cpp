#include "InternalPCH.h"


#include "ShaderResource.h"

#include "Renderer.h"
#include "TextureLoader.h"

#include <DirectXTexEXR.h>
#include <directxtk/DDSTextureLoader.h>

namespace crab
{

//===================================================
// Shader Resource Buffer
//===================================================

void BufferShaderResource::Init(ID3D11Buffer* in_buffer)
{
    m_srv = d3d11::views::CreateSRV(in_buffer, eFormat::Unknown);

    D3D11_BUFFER_DESC desc;
    in_buffer->GetDesc(&desc);

    m_byteWidth    = desc.ByteWidth;
    m_resourceType = eResourceType::Buffer;
    m_buffer       = in_buffer;
}

//===================================================
// Texture 2D
//===================================================

void Texture2D::Init(ID3D11Texture2D* in_texture, eFormat in_format)
{
    D3D11_TEXTURE2D_DESC texDesc;
    in_texture->GetDesc(&texDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = static_cast<DXGI_FORMAT>(in_format);

    if (texDesc.SampleDesc.Count > 1)
    {
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    }
    else
    {
        srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels       = texDesc.MipLevels;
    }

    m_srv              = d3d11::views::CreateSRV(in_texture, in_format);
    m_width            = texDesc.Width;
    m_height           = texDesc.Height;
    m_format           = in_format;
    m_mipMapLevelCount = texDesc.MipLevels;
    m_resourceType     = eResourceType::Texture2D;
    m_texture          = in_texture;
}

void Texture2D::LoadFromFile(
    const std::filesystem::path& in_path,
    eTextureLoadFlags            in_flags)
{
    auto loadData = TextureLoader::LoadFromFile(in_path, in_flags);

    if (!loadData.succeed)
    {
        DEBUG_BREAK("Load ShaderResource Fail.");
        return;
    }

    ComPtr<ID3D11Texture2D> tex;

    if (d3d11::CheckOK(
            DirectX::CreateTexture(
                GetRenderer().GetDevice(),
                loadData.scratchImage.GetImages(),
                loadData.scratchImage.GetImageCount(),
                loadData.metadata,
                reinterpret_cast<ID3D11Resource**>(tex.GetAddressOf())),
            "CreateTexture Fail."))
    {
        Init(tex.Get(), static_cast<eFormat>(loadData.metadata.format));
    }
    else
    {
        DEBUG_BREAK("Load ShaderResource Fail.");
    }
}

//===================================================
// Texture Cube
//===================================================

void TextureCube::Init(const std::array<ID3D11Texture2D*, 6>& in_textures,
                       eFormat                                in_format)
{
    std::vector<ID3D11Texture2D*> textures;
    textures.reserve(6);

    for (auto tex: in_textures)
        textures.push_back(tex);

    ComPtr<ID3D11Texture2D> tex = d3d11::texture::CreateTexture2DArray(
        textures,
        eTextureCreationFlags_CubeMap);

    Init(tex.Get(), in_format);
}

void TextureCube::Init(ID3D11Texture2D* in_textureCube, eFormat in_format)
{
    D3D11_TEXTURE2D_DESC texDesc;
    in_textureCube->GetDesc(&texDesc);

    if (texDesc.MiscFlags != D3D11_RESOURCE_MISC_TEXTURECUBE)
    {
        DEBUG_BREAK("Texture is not CubeMap.");
        return;
    }

    m_srv              = d3d11::views::CreateSRV(in_textureCube, in_format);
    m_width            = texDesc.Width;
    m_height           = texDesc.Height;
    m_format           = in_format;
    m_mipMapLevelCount = texDesc.MipLevels;
    m_resourceType     = eResourceType::TextureCube;
}

void TextureCube::LoadFromFile(
    const std::filesystem::path& in_path,
    eTextureLoadFlags            in_flags)
{
    in_flags |= eTextureLoadFlags_GenerateCubeMap;
    auto loadData = TextureLoader::LoadFromFile(in_path, in_flags);

    if (!loadData.succeed)
    {
        DEBUG_BREAK("Load ShaderResource Fail.");
        return;
    }

    ComPtr<ID3D11Texture2D> tex;

    if (d3d11::CheckOK(
            DirectX::CreateTexture(
                GetRenderer().GetDevice(),
                loadData.scratchImage.GetImages(),
                loadData.scratchImage.GetImageCount(),
                loadData.metadata,
                reinterpret_cast<ID3D11Resource**>(tex.GetAddressOf())),
            "CreateTexture Fail."))
    {
        Init(tex.Get(), static_cast<eFormat>(loadData.metadata.format));
    }
    else
    {
        DEBUG_BREAK("Load ShaderResource Fail.");
    }
}

//===================================================
// Texture 2D Array
//===================================================

void Texture2DArray::Init(ID3D11Texture2D* in_textureArray, eFormat in_format)
{
    D3D11_TEXTURE2D_DESC desc;
    in_textureArray->GetDesc(&desc);

    m_srv          = d3d11::views::CreateSRV(in_textureArray, in_format);
    m_width        = desc.Width;
    m_height       = desc.Height;
    m_format       = in_format;
    m_arraySize    = desc.ArraySize;
    m_resourceType = eResourceType::Texture2DArray;
}

void Texture2DArray::Init(const std::vector<ID3D11Texture2D*>& in_textures, eFormat in_format)
{
    auto tex = d3d11::texture::CreateTexture2DArray(in_textures, eTextureCreationFlags_None);

    D3D11_TEXTURE2D_DESC desc;
    in_textures[0]->GetDesc(&desc);

    Init(tex.Get(), in_format);
}

void ShaderResource::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetShaderResourceView(m_srv.Get(), in_slot, in_bindFlags);
}

ID3D11ShaderResourceView* ShaderResource::GetSRV() const
{
    return m_srv.Get();
}

//===================================================
// Staging Texture
//===================================================

void StagingTexture::Init(
    uint32  in_width,
    uint32  in_height,
    eFormat in_format,
    bool    in_cpuRead,
    bool    in_cpuWrite)
{
    eCPUAccessFlags flags = eCPUAccessFlags_None;

    if (in_cpuRead)
        flags |= eCPUAccessFlags_Read;

    if (in_cpuWrite)
        flags |= eCPUAccessFlags_Write;

    ComPtr<ID3D11Texture2D>
        tex = d3d11::texture::CreateTexture2D(
            in_width,
            in_height,
            in_format,
            D3D11_USAGE_STAGING,
            eBindFlags_None,
            flags,
            MSAA::DisableMSAA(),
            1,
            1,
            eTextureCreationFlags_None,
            nullptr);

    m_height  = in_height;
    m_width   = in_width;
    m_format  = in_format;
    m_texture = tex;
}

void StagingTexture::WriteToTexture(
    const void* in_writeData,
    uint32      in_writeDataByteWidth) const
{
    d3d11::texture::WriteToStagingTexture(
        m_texture.Get(),
        in_writeData,
        in_writeDataByteWidth);
}

void StagingTexture::ReadFromTexture(
    void*   out_data,
    uint32* out_dataByteWidth) const
{
    d3d11::texture::ReadFromStagingTexture(
        m_texture.Get(),
        out_data,
        out_dataByteWidth);
}

void StagingTexture::CopyTo(ID3D11Texture2D* in_destTexture) const
{
    d3d11::texture::CopyBetween(
        m_texture.Get(),
        in_destTexture);
}

void StagingTexture::CopyFrom(ID3D11Texture2D* in_srcTexture) const
{
    d3d11::texture::CopyBetween(
        in_srcTexture,
        m_texture.Get());
}

}   // namespace crab