#include "CrabPch.h"

#include "D11_Texture.h"

#include "D11_RenderState.h"
#include "D11_Renderer.h"
#include "D11_Swapchain.h"
#include <DirectXTexEXR.h>
#include <directxtk/DDSTextureLoader.h>

namespace crab
{

Ref<D11_Texture2D> D11_Texture2D::Create(
    ID3D11Texture2D*                       in_texture,
    const D3D11_SHADER_RESOURCE_VIEW_DESC& in_desc)
{
    auto               d   = D11_API->GetDevice();
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    D11_ASSERT(d->CreateShaderResourceView(in_texture,
                                           &in_desc,
                                           tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::Create(const std::filesystem::path& in_path)
{
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    const auto         d   = D11_API->GetDevice();

    DirectX::ScratchImage image;
    DirectX::TexMetadata  metadata;

    if (!D11_TextureLoader::LoadImageFromFile(in_path, &image, &metadata))
    {
        CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
        return nullptr;
    }

    const DirectX::Image* images = image.GetImages();
    D11_ASSERT(DirectX::CreateShaderResourceView(d.Get(),
                                                 images,
                                                 1,
                                                 metadata,
                                                 tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = *image.GetImages();

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureArray(const std::vector<std::filesystem::path>& in_paths)
{
    using namespace DirectX;
    std::vector<ScratchImage> scratchImages(in_paths.size());
    TexMetadata               stdMetadata = {};

    for (size_t i = 0; i < in_paths.size(); i++)
    {
        ScratchImage srcImage = {};
        TexMetadata  metadata = {};

        if (!D11_TextureLoader::LoadImageFromFile(in_paths[i], &srcImage, &metadata))
        {
            CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
            return nullptr;
        }

        if (i == 0)
        {
            stdMetadata = metadata;
        }
        else
        {
            if (stdMetadata.width != metadata.width ||
                stdMetadata.height != metadata.height ||
                stdMetadata.format != metadata.format)
            {
                CRAB_DEBUG_BREAK("Texture Array Size Mismatch.");
                return nullptr;
            }
        }
    }

    std::vector<Image> images(in_paths.size());
    for (size_t i = 0; i < scratchImages.size(); i++)
        images[i] = *scratchImages[i].GetImage(0, 0, 0);

    stdMetadata.arraySize  = in_paths.size();
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    auto               d   = D11_API->GetDevice();
    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   stdMetadata,
                   tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = images[0];
    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateMipmapTextureArray(const std::vector<std::filesystem::path>& in_paths, uint32 in_mipLevel)
{
    using namespace DirectX;
    auto                      dx = D11_API->GetDevice();
    TexMetadata               metadata;
    ScratchImage              srcImage;
    std::vector<ScratchImage> mipChain(in_paths.size());

    for (uint32 i = 0; i < in_paths.size(); ++i)
    {
        if (!D11_TextureLoader::LoadImageFromFile(in_paths[i], &srcImage, &metadata))
        {
            CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
            return nullptr;
        }

        D11_ASSERT(GenerateMipMaps(srcImage.GetImages(),
                                   1,
                                   metadata,
                                   TEX_FILTER_DEFAULT,
                                   in_mipLevel,
                                   mipChain[i]),
                   "GenerateMipMaps Fail.");
    }

    std::vector<Image> images(in_paths.size());
    for (uint32 i = 0; i < in_paths.size(); ++i)
    {
        images[i] = *mipChain[i].GetImages();
    }

    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    D11_ASSERT(CreateShaderResourceView(dx.Get(),
                                        images.data(),
                                        metadata.arraySize,
                                        metadata,
                                        tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = images[0];
    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureCubeArray(const std::vector<std::filesystem::path>& in_paths)
{
    using namespace DirectX;
    auto dx = D11_API->GetDevice();

    ScratchImage image;
    TexMetadata  metadata;
    for (uint32 i = 0; i < in_paths.size(); i++)
    {
        if (in_paths[i].extension() != ".dds")
        {
            CRAB_DEBUG_BREAK("TextureCube must be DDS format.");
            return nullptr;
        }
        if (!D11_TextureLoader::LoadImageFromFile(in_paths[i], &image, &metadata))
        {
            CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
            return nullptr;
        }
    }
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    D11_ASSERT(CreateShaderResourceViewEx(dx.Get(),
                                          image.GetImages(),
                                          1,
                                          metadata,
                                          D3D11_USAGE_IMMUTABLE,
                                          D3D11_BIND_SHADER_RESOURCE,
                                          0,
                                          D3D11_RESOURCE_MISC_TEXTURECUBE,
                                          CREATETEX_DEFAULT,
                                          tex->m_srv.GetAddressOf()),
               "CreateShaderResourceViewEx Fail.");
    tex->m_image = *image.GetImages();

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateMipmap(const std::filesystem::path& in_path, uint32 in_mipLevel)
{
    using namespace DirectX;
    auto dx = D11_API->GetDevice();

    TexMetadata  metadata;
    ScratchImage srcImage;

    if (!D11_TextureLoader::LoadImageFromFile(in_path, &srcImage, &metadata))
    {
        CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
        return nullptr;
    }

    ScratchImage mipChain;
    D11_ASSERT(GenerateMipMaps(srcImage.GetImages(),
                               1,
                               metadata,
                               TEX_FILTER_DEFAULT,
                               in_mipLevel,
                               mipChain),
               "GenerateMipMaps Fail.");

    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    D11_ASSERT(CreateShaderResourceView(dx.Get(),
                                        mipChain.GetImages(),
                                        mipChain.GetImageCount(),
                                        mipChain.GetMetadata(),
                                        tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = *mipChain.GetImages();

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureCube(const std::filesystem::path& in_path)
{
    using namespace DirectX;
    auto dx = D11_API->GetDevice();

    if (in_path.extension() != ".dds")
    {
        CRAB_DEBUG_BREAK("TextureCube must be DDS format.");
        return nullptr;
    }

    ScratchImage image;
    TexMetadata  metadata;

    if (!D11_TextureLoader::LoadImageFromFile(in_path, &image, &metadata))
    {
        CRAB_DEBUG_BREAK("LoadImageFromFile Fail.");
        return nullptr;
    }

    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    CRAB_ASSERT(CreateShaderResourceViewEx(dx.Get(),
                                           image.GetImages(),
                                           1,
                                           metadata,
                                           D3D11_USAGE_IMMUTABLE,
                                           D3D11_BIND_SHADER_RESOURCE,
                                           0,
                                           D3D11_RESOURCE_MISC_TEXTURECUBE,
                                           CREATETEX_DEFAULT,
                                           tex->m_srv.GetAddressOf()),
                "CreateShaderResourceViewEx Fail.");

    tex->m_image = *image.GetImages();
    return tex;
}

void D11_Texture2D::Bind(uint32 in_slot, eShaderFlags in_flag) const
{
    auto* dx = D11_API;
    dx->SetShaderResourceView(m_srv.Get(), in_slot, in_flag);
}

ID3D11ShaderResourceView* D11_Texture2D::GetSRV() const
{
    return m_srv.Get();
}

void D11_TextureList::ClearList()
{
    m_nodes.clear();
}

D11_TextureList& D11_TextureList::Add(Ref<D11_Texture2D> in_texture, uint32 in_slot, eShaderFlags in_flag)
{
    m_nodes.push_back({ in_texture, in_slot, in_flag });
    return *this;
}

Ref<D11_Texture2D> D11_TextureList::GetTexture(uint32 in_index) const
{
    return m_nodes[in_index].texture;
}

void D11_TextureList::Bind() const
{
    for (auto& node: m_nodes)
        node.texture->Bind(node.slot, node.flag);
}

bool D11_TextureLoader::LoadImageFromFile(
    const std::filesystem::path& in_path,
    DirectX::ScratchImage*       out_scratchImage,
    DirectX::TexMetadata*        out_metaData)
{
    using namespace DirectX;

    const std::filesystem::path ext = in_path.extension();
    ScratchImage                image;
    TexMetadata                 metadata;

    Log::Info("Load Image: {0}", in_path.string());

    if (ext == ".dds")
    {
        if (FAILED(LoadFromDDSFile(in_path.c_str(), DDS_FLAGS_NONE, &metadata, image)))
        {
            CRAB_DEBUG_BREAK("LoadFromDDSFile Fail.");
            return false;
        }
    }
    if (ext == ".exr")
    {
        if (FAILED(LoadFromEXRFile(in_path.c_str(), &metadata, image)))
        {
            CRAB_DEBUG_BREAK("LoadFromEXRFile Fail.");
            return false;
        }
    }
    else
    {
        WIC_FLAGS flags = WIC_FLAGS_NONE;

        if (D11_API->GetSwapChain()->HasFloatRenderTarget())
            flags = WIC_FLAGS_DEFAULT_SRGB;

        if (FAILED(LoadFromWICFile(in_path.c_str(), flags, &metadata, image)))
        {
            CRAB_DEBUG_BREAK("LoadFromWICFile Fail.");
            return false;
        }
    }

    if (out_scratchImage)
        *out_scratchImage = std::move(image);

    if (out_metaData)
        *out_metaData = metadata;

    return true;
}

}   // namespace crab