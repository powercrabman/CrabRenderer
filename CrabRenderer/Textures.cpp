#include "CrabPch.h"

#include "Textures.h"

#include "D11Renderer.h"
#include "ImageLoader.h"
#include "RenderStates.h"
#include "Swapchain.h"

#include <DirectXTexEXR.h>
#include <directxtk/DDSTextureLoader.h>

namespace crab
{

Ref<Image2D> Image2D::CreateFromTexture(ID3D11Texture2D* in_texture)
{
    return CreateFromTexture(in_texture, eFormat::Unknown);
}

Ref<Image2D> Image2D::CreateFromTexture(
    ID3D11Texture2D* in_texture,
    eFormat          in_format)
{
    D3D11_TEXTURE2D_DESC texDesc;
    in_texture->GetDesc(&texDesc);

    if (in_format == eFormat::Unknown)
        in_format = static_cast<eFormat>(texDesc.Format);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format                          = static_cast<DXGI_FORMAT>(in_format);

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

    ComPtr<ID3D11ShaderResourceView> srv;
    CheckD3D11Result(
        GetRenderer().GetDevice()->CreateShaderResourceView(
            in_texture,
            &srvDesc,
            srv.GetAddressOf()),
        "CreateShaderResourceView Fail.");

    Ref<Image2D> image        = CreateRef<Image2D>();
    image->m_srv              = srv;
    image->m_width            = texDesc.Width;
    image->m_height           = texDesc.Height;
    image->m_format           = in_format;
    image->m_mipMapLevelCount = texDesc.MipLevels;

    return image;
}

Ref<Image2D> Image2D::CreateFromBuffer(ID3D11Buffer* in_buffer)
{
    return CreateFromBuffer(in_buffer, eFormat::Unknown);
}

Ref<Image2D> Image2D::CreateFromBuffer(ID3D11Buffer* in_buffer, eFormat in_format)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    in_buffer->GetDesc(&bufferDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format                          = static_cast<DXGI_FORMAT>(in_format);
    srvDesc.ViewDimension                   = D3D11_SRV_DIMENSION_BUFFEREX;
    srvDesc.BufferEx.FirstElement           = 0;
    srvDesc.BufferEx.NumElements            = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
    srvDesc.BufferEx.Flags                  = 0;

    ComPtr<ID3D11ShaderResourceView> srv;
    CheckD3D11Result(
        GetRenderer().GetDevice()->CreateShaderResourceView(
            in_buffer,
            &srvDesc,
            srv.GetAddressOf()),
        "CreateShaderResourceView Fail.");

    Ref<Image2D> image        = CreateRef<Image2D>();
    image->m_srv              = srv;
    image->m_width            = srvDesc.BufferEx.NumElements;
    image->m_height           = 1;
    image->m_format           = in_format;
    image->m_mipMapLevelCount = 1;

    return image;
}

Ref<Image2D> Image2D::CreateFromFile(
    const std::filesystem::path& in_path,
    bool                         in_enableMipMap,
    bool                         in_inverseToneMapping)
{
    Ref<Image2D> image    = nullptr;
    auto         loadData = in_inverseToneMapping ?
                                ImageLoader::LoadFromFileWICEx(in_path, DirectX::WIC_FLAGS::WIC_FLAGS_DEFAULT_SRGB) :
                                ImageLoader::LoadFromFile(in_path);

    if (loadData.succeed)
    {
        ComPtr<ID3D11ShaderResourceView> srv;

        loadData.metadata.mipLevels = in_enableMipMap ? loadData.metadata.mipLevels : 1;

        // generate mipmap
        if (in_enableMipMap && loadData.metadata.mipLevels == 1)
            ImageLoader::GenerateMipmap(loadData);

        if (CheckD3D11Result(
                DirectX::CreateShaderResourceView(
                    GetRenderer().GetDevice().Get(),
                    loadData.scratchImage.GetImages(),
                    loadData.scratchImage.GetImageCount(),
                    loadData.metadata,
                    srv.GetAddressOf())))
        {
            image                     = CreateRef<Image2D>();
            image->m_srv              = srv;
            image->m_width            = static_cast<uint32>(loadData.metadata.width);
            image->m_height           = static_cast<uint32>(loadData.metadata.height);
            image->m_format           = static_cast<eFormat>(loadData.metadata.format);
            image->m_mipMapLevelCount = static_cast<uint32>(loadData.metadata.mipLevels);
        }
    }
    else
    {
        CRAB_DEBUG_BREAK("Load Texture Fail.");
    }

    return image;
}

Ref<Image2D> Image2D::CreateTextureArrayFromFile(
    const std::vector<std::filesystem::path>& in_paths,
    bool                                      in_enableMipMap,
    bool                                      in_inverseToneMapping)
{
    Ref<Image2D> image = nullptr;

    if (in_paths.size() > 0)
    {
        DirectX::TexMetadata standardMetadata;

        std::vector<DirectX::Image> images;

        for (uint32 i = 0; i < in_paths.size(); i++)
        {
            auto loadData = in_inverseToneMapping ?
                                ImageLoader::LoadFromFileWICEx(in_paths[i], DirectX::WIC_FLAGS::WIC_FLAGS_DEFAULT_SRGB) :
                                ImageLoader::LoadFromFile(in_paths[i]);

            if (loadData.succeed)
            {
                // check image format
                if (i == 0)
                {
                    standardMetadata = loadData.metadata;
                }
                else
                {
                    if (standardMetadata.width != loadData.metadata.width || standardMetadata.height != loadData.metadata.height || standardMetadata.format != loadData.metadata.format)
                    {
                        CRAB_DEBUG_BREAK("Texture size is not same.");
                        return nullptr;
                    }
                }

                // generate mipmap
                if (in_enableMipMap && loadData.metadata.mipLevels == 1)
                {
                    ImageLoader::GenerateMipmap(loadData);
                    standardMetadata = loadData.metadata;
                }

                images.push_back(*loadData.scratchImage.GetImage(0, 0, 0));
            }
            else
            {
                CRAB_DEBUG_BREAK("Load Texture Fail.");
            }
        }

        DirectX::TexMetadata metadata;
        metadata.width      = standardMetadata.width;
        metadata.height     = standardMetadata.height;
        metadata.depth      = 1;
        metadata.arraySize  = static_cast<uint32>(in_paths.size());
        metadata.mipLevels  = in_enableMipMap ? standardMetadata.mipLevels : 1;
        metadata.format     = standardMetadata.format;
        metadata.dimension  = DirectX::TEX_DIMENSION_TEXTURE2D;
        metadata.miscFlags  = 0;
        metadata.miscFlags2 = 0;

        ComPtr<ID3D11ShaderResourceView> srv;

        if (CheckD3D11Result(
                DirectX::CreateShaderResourceView(
                    GetRenderer().GetDevice().Get(),
                    images.data(),
                    static_cast<uint32>(images.size()),
                    metadata,
                    srv.GetAddressOf())))
        {
            image                     = CreateRef<Image2D>();
            image->m_srv              = srv;
            image->m_width            = (uint32)metadata.width;
            image->m_height           = (uint32)metadata.height;
            image->m_format           = static_cast<eFormat>(metadata.format);
            image->m_mipMapLevelCount = (uint32)metadata.mipLevels;
        }
    }

    return image;
}

Ref<Image2D> Image2D::CreateTextureArrayFromFile(
    const std::filesystem::path& in_path,
    uint32                       in_arrayMaxCount,
    bool                         in_enableMipMap,
    bool                         in_inverseToneMapping)
{
    Ref<Image2D> image = nullptr;

    auto loadData = in_inverseToneMapping ?
                        ImageLoader::LoadFromFileWICEx(in_path, DirectX::WIC_FLAGS::WIC_FLAGS_DEFAULT_SRGB) :
                        ImageLoader::LoadFromFile(in_path);

    DirectX::Image srcImage = *loadData.scratchImage.GetImage(0, 0, 0);

    uint64 width  = static_cast<uint64>(loadData.metadata.width / static_cast<float>(in_arrayMaxCount));
    uint64 height = loadData.metadata.height;

    // mipmap
    if (in_enableMipMap && loadData.metadata.mipLevels == 1)
    {
        ImageLoader::GenerateMipmap(loadData);
        srcImage = *loadData.scratchImage.GetImage(0, 0, 0);
    }

    std::vector<DirectX::Image> dstImages;
    dstImages.reserve(in_arrayMaxCount);

    for (uint32 i = 0; i < in_arrayMaxCount; ++i)
    {
        DirectX::Image dstImage;

        DirectX::Rect rect;
        rect.w = width;
        rect.h = height;
        rect.x = width * i;
        rect.y = 0;

        if (CheckD3D11Result(
                DirectX::CopyRectangle(
                    srcImage,
                    rect,
                    dstImage,
                    DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT,
                    0,
                    0)),
            "CopyRectangle Fail.")
        {
            dstImages.push_back(dstImage);
        }
        else
        {
            CRAB_DEBUG_BREAK("CopyRectangle Fail.");
            return nullptr;
        }
    }

    DirectX::TexMetadata metadata;
    metadata.width      = width;
    metadata.height     = height;
    metadata.depth      = 1;
    metadata.arraySize  = in_arrayMaxCount;
    metadata.mipLevels  = in_enableMipMap ? loadData.metadata.mipLevels : 1;
    metadata.format     = loadData.metadata.format;
    metadata.dimension  = DirectX::TEX_DIMENSION_TEXTURE2D;
    metadata.miscFlags  = 0;
    metadata.miscFlags2 = 0;

    ComPtr<ID3D11ShaderResourceView> srv;

    if (CheckD3D11Result(
            DirectX::CreateShaderResourceView(
                GetRenderer().GetDevice().Get(),
                dstImages.data(),
                static_cast<uint32>(dstImages.size()),
                metadata,
                srv.GetAddressOf()),
            "CreateShaderResourceView Fail."))
    {
        image                     = CreateRef<Image2D>();
        image->m_srv              = srv;
        image->m_width            = (uint32)metadata.width;
        image->m_height           = (uint32)metadata.height;
        image->m_format           = static_cast<eFormat>(metadata.format);
        image->m_mipMapLevelCount = (uint32)metadata.mipLevels;
    }

    return image;
}

void Image2D::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    GetRenderer().SetShaderResourceView(m_srv.Get(), in_slot, in_bindFlags);
}

ID3D11ShaderResourceView* Image2D::GetSRV() const
{
    return m_srv.Get();
}

bool Image2D::IsTextureCube() const
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    m_srv->GetDesc(&srvDesc);

    return srvDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURECUBE;
}

Ref<StagingTexture2D> StagingTexture2D::Create(
    uint32  in_width,
    uint32  in_height,
    eFormat in_format,
    bool    in_cpuRead,
    bool    in_cpuWrite)
{
    ComPtr<ID3D11Texture2D> tex = ID3D11Texture2DUtil::CreateStagingTexture2D(
        in_width,
        in_height,
        in_format,
        in_cpuRead,
        in_cpuWrite,
        1);

    Ref<StagingTexture2D> stagingTex = CreateRef<StagingTexture2D>();

    stagingTex->m_height  = in_height;
    stagingTex->m_width   = in_width;
    stagingTex->m_format  = in_format;
    stagingTex->m_texture = tex;

    return stagingTex;
}

void StagingTexture2D::WriteToTexture(
    void*  in_data,
    uint32 in_pixelStride,
    uint32 in_width,
    uint32 in_height) const
{
    ID3D11Texture2DUtil::WriteToStagingTexture(
        m_texture.Get(),
        in_data,
        in_pixelStride,
        in_width,
        in_height);
}

void StagingTexture2D::ReadFromTexture(void* out_data) const
{
    ID3D11Texture2DUtil::ReadFromStagingTexture(
        m_texture.Get(),
        out_data);
}

void StagingTexture2D::CopyTo(ID3D11Texture2D* in_destTexture) const
{
    ID3D11Texture2DUtil::CopyBetween(
        m_texture.Get(),
        in_destTexture);
}

void StagingTexture2D::CopyFrom(ID3D11Texture2D* in_srcTexture) const
{
    ID3D11Texture2DUtil::CopyBetween(
        in_srcTexture,
        m_texture.Get());
}

Ref<CubemapImage> CubemapImage::Create(const std::filesystem::path& in_path)
{
    // todo: test code necessary (png, exr, dds)
    Ref<Image2D> image = Image2D::CreateFromFile(in_path, true);

    Ref<CubemapImage> skybox = CreateRef<CubemapImage>();
    skybox->m_image          = image;

    return skybox;
}

void CubemapImage::Bind(uint32 in_slot, eShaderFlags in_bindFlags) const
{
    m_image->Bind(in_slot, in_bindFlags);
}

}   // namespace crab