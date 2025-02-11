#include "CrabPch.h"

#include "D11_Texture.h"

#include "D11_RenderState.h"
#include "D11_Renderer.h"
#include <DirectXTexEXR.h>
#include <directxtk/DDSTextureLoader.h>
#include "D11_Swapchain.h"

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
    auto               d   = D11_API->GetDevice();

    DirectX::ScratchImage image    = D11_TextureUtile::LoadImageFromFile(in_path);
    DirectX::TexMetadata  metadata = image.GetMetadata();
    const DirectX::Image* images   = image.GetImages();
    D11_ASSERT(DirectX::CreateShaderResourceView(d.Get(),
                                                 images,
                                                 1,
                                                 metadata,
                                                 tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = *image.GetImages();

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureCube(
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
        scratchImages[i] = D11_TextureUtile::LoadImageFromFile(pathes[i]);

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

    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    auto               d   = D11_API->GetDevice();

    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   metadata,
                   tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = images[0];

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureCubeByDDS(const std::filesystem::path& in_path)
{
    using namespace DirectX;
    auto d3dDevice = D11_API->GetDevice();

    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();

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
    tex->m_image.width      = desc.Width;
    tex->m_image.height     = desc.Height;
    tex->m_image.format     = desc.Format;
    tex->m_image.rowPitch   = static_cast<uint64>(desc.Width) * 4;
    tex->m_image.slicePitch = tex->m_image.rowPitch * desc.Height;

    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateTextureArray(const std::vector<std::filesystem::path>& in_paths)
{
    using namespace DirectX;
    std::vector<ScratchImage> scratchImages(in_paths.size());
    TexMetadata               metadata = {};

    for (size_t i = 0; i < in_paths.size(); i++)
    {
        scratchImages[i] = D11_TextureUtile::LoadImageFromFile(in_paths[i]);

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
    metadata.arraySize     = in_paths.size();
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    auto               d   = D11_API->GetDevice();
    D11_ASSERT(CreateShaderResourceView(
                   d.Get(),
                   images.data(),
                   images.size(),
                   metadata,
                   tex->m_srv.GetAddressOf()),
               "CreateShaderResourceView Fail.");

    tex->m_image = images[0];
    return tex;
}

Ref<D11_Texture2D> D11_Texture2D::CreateMipmap(const std::filesystem::path& in_path, uint32 in_mipLevel)
{
    using namespace DirectX;
    auto dx = D11_API->GetDevice();

    ScratchImage srcImage = D11_TextureUtile::LoadImageFromFile(in_path);
    TexMetadata  metadata = srcImage.GetMetadata();

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

void D11_Texture2D::Bind(uint32 in_slot, eShaderFlags in_flag)
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

void D11_TextureList::Bind()
{
    for (auto& node: m_nodes)
        node.texture->Bind(node.slot, node.flag);
}

DirectX::ScratchImage D11_TextureUtile::LoadImageFromFile(const std::filesystem::path& in_path)
{
    using namespace DirectX;

    const std::filesystem::path ext = in_path.extension();
    ScratchImage                image;

    if (ext == ".dds")
    {
        D11_ASSERT(LoadFromDDSFile(in_path.c_str(), DDS_FLAGS_NONE, nullptr, image), "LoadFromDDSFile Fail.");
        return image;
    }
    if (ext == ".exr")
    {
        D11_ASSERT(LoadFromEXRFile(in_path.c_str(), nullptr, image), "LoadFromEXRFile Fail.");
        return image;
    }
    else
    {
        WIC_FLAGS flags = WIC_FLAGS_NONE;

        if (D11_API->GetSwapChain()->HasFloatRenderTarget())
            flags = WIC_FLAGS_DEFAULT_SRGB;

        D11_ASSERT(LoadFromWICFile(in_path.c_str(), flags, nullptr, image), "LoadFromWICFile Fail.");
        return image;
    }
}

}   // namespace crab