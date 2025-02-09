#include "CrabPch.h"

#include "D11_Texture.h"

#include "D11_RenderState.h"
#include "D11_Renderer.h"
#include <directxtk/DDSTextureLoader.h>

namespace crab
{

Ref<D11_Texture> D11_Texture::Create(
    ID3D11Texture2D*                       in_texture,
    const D3D11_SHADER_RESOURCE_VIEW_DESC& in_desc)
{
    auto             d   = D11_API->GetDevice();
    Ref<D11_Texture> tex = CreateRef<D11_Texture>();
    D11_ASSERT(d->CreateShaderResourceView(in_texture, &in_desc, tex->m_srv.GetAddressOf()), "CreateShaderResourceView Fail.");
    return tex;
}

Ref<D11_Texture> D11_Texture::Create(const std::filesystem::path& in_path)
{
    Ref<D11_Texture> tex = CreateRef<D11_Texture>();
    auto             d   = D11_API->GetDevice();

    DirectX::ScratchImage image;
    D11_ASSERT(DirectX::LoadFromWICFile(in_path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image), "LoadFromWICFile Fail.");

    DirectX::TexMetadata  metadata = image.GetMetadata();
    const DirectX::Image* images   = image.GetImages();
    D11_ASSERT(DirectX::CreateShaderResourceView(d.Get(),
                                                 images,
                                                 1,
                                                 metadata,
                                                 tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_imageData = metadata;

    return tex;
}

Ref<D11_Texture> D11_Texture::CreateTextureCube(
    const std::filesystem::path& in_pathPX,
    const std::filesystem::path& in_pathNX,
    const std::filesystem::path& in_pathPY,
    const std::filesystem::path& in_pathNY,
    const std::filesystem::path& in_pathPZ,
    const std::filesystem::path& in_pathNZ)
{
    using namespace DirectX;

    std::array<std::filesystem::path, 6> pathes = {
        in_pathPX,
        in_pathNX,
        in_pathPY,
        in_pathNY,
        in_pathPZ,
        in_pathNZ,
    };
    std::array<ScratchImage, 6> scratchImages;
    TexMetadata                 metadata = {};

    for (size_t i = 0; i < pathes.size(); i++)
    {
        D11_ASSERT_V(LoadFromWICFile(pathes[i].c_str(), WIC_FLAGS_NONE, nullptr, scratchImages[i]),
                     "LoadFromWICFile Fail: {}",
                     pathes[i].string().c_str());

        if (i == 0)
        {
            metadata = scratchImages[i].GetMetadata();
        }
        else
        {
            TexMetadata metaTemp = scratchImages[i].GetMetadata();
            CRAB_ASSERT(metadata.width == metaTemp.width &&
                            metadata.height == metaTemp.height &&
                            metadata.format == metaTemp.format,
                        "All images must have the same format and dimension");
        }
    }

    std::array<Image, 6> images = {};
    for (size_t i = 0; i < scratchImages.size(); i++)
    {
        images[i] = *scratchImages[i].GetImage(0, 0, 0);
    }

    metadata.arraySize = 6;
    metadata.miscFlags |= TEX_MISC_TEXTURECUBE;

    Ref<D11_Texture> tex = CreateRef<D11_Texture>();
    auto             d   = D11_API->GetDevice();

    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   metadata,
                   tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_imageData = metadata;

    return tex;
}

Ref<D11_Texture> D11_Texture::CreateTextureCubeByDDS(const std::filesystem::path& in_path)
{
    using namespace DirectX;
    auto d3dDevice = D11_API->GetDevice();

    Ref<D11_Texture> tex = CreateRef<D11_Texture>();

    ComPtr<ID3D11Resource> resource;
    DDS_ALPHA_MODE         alphaMode = DDS_ALPHA_MODE_UNKNOWN;

    D11_ASSERT(CreateDDSTextureFromFileEx(
                   d3dDevice.Get(),
                   in_path.c_str(),
                   0,
                   D3D11_USAGE_DEFAULT,
                   D3D11_BIND_SHADER_RESOURCE,
                   0,
                   D3D11_RESOURCE_MISC_TEXTURECUBE,
                   DDS_LOADER_DEFAULT,
                   resource.GetAddressOf(),
                   tex->m_srv.GetAddressOf(),
                   nullptr),
               "CreateDDSTextureFromFileEx Fail.");

    ComPtr<ID3D11Texture2D> tex2D;
    D11_ASSERT(resource.As(&tex2D), "As Fail.");

    D3D11_TEXTURE2D_DESC desc = {};
    tex2D->GetDesc(&desc);
    tex->m_imageData.width     = desc.Width;
    tex->m_imageData.height    = desc.Height;
    tex->m_imageData.mipLevels = desc.MipLevels;
    tex->m_imageData.format    = desc.Format;

    return tex;
}

Ref<D11_Texture> D11_Texture::CreateTexture2DArray(const std::vector<std::filesystem::path>& in_paths)
{
    using namespace DirectX;
    std::vector<ScratchImage> scratchImages(in_paths.size());
    TexMetadata               metadata = {};

    for (size_t i = 0; i < in_paths.size(); i++)
    {
        D11_ASSERT_V(LoadFromWICFile(in_paths[i].c_str(), WIC_FLAGS_NONE, nullptr, scratchImages[i]),
                     "LoadFromWICFile Fail: {}",
                     in_paths[i].string().c_str());
        if (i == 0)
        {
            metadata = scratchImages[i].GetMetadata();
        }
        else
        {
            TexMetadata metaTemp = scratchImages[i].GetMetadata();
            CRAB_ASSERT(metadata.width == metaTemp.width &&
                            metadata.height == metaTemp.height &&
                            metadata.format == metaTemp.format,
                        "All images must have the same format and dimension");
        }
    }
    std::vector<Image> images(in_paths.size());
    for (size_t i = 0; i < scratchImages.size(); i++)
    {
        images[i] = *scratchImages[i].GetImage(0, 0, 0);
    }
    metadata.arraySize   = in_paths.size();
    Ref<D11_Texture> tex = CreateRef<D11_Texture>();
    auto             d   = D11_API->GetDevice();
    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   metadata,
                   tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");
    tex->m_imageData = metadata;
    return tex;
}

void D11_Texture::Bind(uint32 in_slot, eShaderFlags in_flag)
{
    auto* dx = D11_API;
    dx->SetShaderResourceView(m_srv.Get(), in_slot, in_flag);
}

ID3D11ShaderResourceView* D11_Texture::Get() const
{
    return m_srv.Get();
}

}   // namespace crab
