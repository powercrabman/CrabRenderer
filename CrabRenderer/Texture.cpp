#include "CrabPch.h"

#include "Texture.h"

#include "D11_RenderState.h"
#include "D11_Renderer.h"
#include <directxtk/DDSTextureLoader.h>

namespace crab
{

Ref<Texture> Texture::Create(const std::filesystem::path& in_path,
                             const Ref<D11_SamplerState>& in_samplerState)
{
    Ref<crab::Texture> tex = CreateRef<Texture>();
    auto               d   = D11_API->GetDevice();

    DirectX::ScratchImage image;
    D11_ASSERT(DirectX::LoadFromWICFile(in_path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image), "LoadFromWICFile Fail.");

    DirectX::TexMetadata  metadata = image.GetMetadata();
    const DirectX::Image* images   = image.GetImages();
    D11_ASSERT(DirectX::CreateShaderResourceView(d.Get(),
                                                 images,
                                                 1,
                                                 metadata,
                                                 tex->srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->imageData    = metadata;
    tex->samplerState = in_samplerState;

    return tex;
}

Ref<Texture> Texture::CreateTextureCube(
    const std::filesystem::path& in_pathPX,
    const std::filesystem::path& in_pathNX,
    const std::filesystem::path& in_pathPY,
    const std::filesystem::path& in_pathNY,
    const std::filesystem::path& in_pathPZ,
    const std::filesystem::path& in_pathNZ,
    const Ref<D11_SamplerState>& in_samplerState)
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

    Ref<Texture> tex = CreateRef<Texture>();
    auto         d   = D11_API->GetDevice();

    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   metadata,
                   tex->srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->imageData    = metadata;
    tex->samplerState = in_samplerState;

    return tex;
}

Ref<Texture> Texture::CreateTextureCubeByDDS(
    const std::filesystem::path& in_path,
    const Ref<D11_SamplerState>& in_samplerState)
{
    using namespace DirectX;
    auto d3dDevice = D11_API->GetDevice();

    Ref<Texture> tex = CreateRef<Texture>();

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
                   tex->srv.GetAddressOf(),
                   nullptr),
               "CreateDDSTextureFromFileEx Fail.");

    ComPtr<ID3D11Texture2D> tex2D;
    D11_ASSERT(resource.As(&tex2D), "As Fail.");

    D3D11_TEXTURE2D_DESC desc = {};
    tex2D->GetDesc(&desc);
    tex->imageData.width     = desc.Width;
    tex->imageData.height    = desc.Height;
    tex->imageData.mipLevels = desc.MipLevels;
    tex->imageData.format    = desc.Format;
    tex->samplerState        = in_samplerState;

    return tex;
}

TextureArray::TextureArray(const std::vector<Ref<Texture>>& in_textures)
    : textures(in_textures)
{
    srvPtrs.reserve(in_textures.size());
    statePtrs.reserve(in_textures.size());
    for (const auto& tex: in_textures)
    {
        srvPtrs.push_back(tex->srv.Get());
        statePtrs.push_back(tex->samplerState->samplerState.Get());
    }
}

TextureArray& crab::TextureArray::Add(const Ref<Texture>& in_texture)
{
    textures.push_back(in_texture);
    srvPtrs.push_back(in_texture->srv.Get());
    statePtrs.push_back(in_texture->samplerState->samplerState.Get());
    return *this;
}

void TextureArray::Clear()
{
    textures.clear();
    srvPtrs.clear();
    statePtrs.clear();
}

}   // namespace crab
