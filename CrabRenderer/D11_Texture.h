#pragma once
#include "D11_Utiles.h"

namespace crab
{

struct D11_Texture1D;
struct D11_Texture2D;
struct D11_Texture3D;

// Helper class
struct D11_Texture
{
public:
    static Ref<D11_Texture1D> Create1D(const D3D11_TEXTURE1D_DESC& in_desc);
    static Ref<D11_Texture2D> Create2D(const D3D11_TEXTURE2D_DESC& in_desc);
    static Ref<D11_Texture3D> Create3D(const D3D11_TEXTURE3D_DESC& in_desc);

    static Ref<D11_Texture1D> Create1DFromFile(const std::filesystem::path& in_path);
    static Ref<D11_Texture2D> Create2DFromFile(const std::filesystem::path& in_path);
    static Ref<D11_Texture3D> Create3DFromFile(const std::filesystem::path& in_path);

private:
    template<typename Ty>
    static Ref<Ty> _CreateFromFileHelper(const std::filesystem::path& in_path)
    {
        Ref<Ty> tex = CreateRef<Ty>();
        auto    d   = D11_API->GetDevice();

        DirectX::ScratchImage image;
        D11_ASSERT(DirectX::LoadFromWICFile(in_path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image),
                    "LoadFromWICFile Fail.");

        DirectX::TexMetadata   metadata = image.GetMetadata();
        const DirectX::Image*  images   = image.GetImages();
        ComPtr<ID3D11Resource> resource;

        D11_ASSERT(DirectX::CreateTexture(d.Get(), images, 1, metadata, resource.GetAddressOf()),
                    "CreateTexture Fail.");

        D11_ASSERT(resource.As(&tex->texture), "As Fail.");

        return tex;
    }
};

// Concrete class
struct D11_Texture1D : public D11_Texture
{
    ComPtr<ID3D11Texture1D> texture;
    D3D11_TEXTURE1D_DESC    desc;
};

struct D11_Texture2D : public D11_Texture
{
    ComPtr<ID3D11Texture2D> texture;
    D3D11_TEXTURE2D_DESC    desc;
};

struct D11_Texture3D : public D11_Texture
{
    ComPtr<ID3D11Texture3D> texture;
    D3D11_TEXTURE3D_DESC    desc;
};

}   // namespace crab