#include "CrabPch.h"

#include "D11_Renderer.h"
#include "D11_Texture.h"

namespace crab
{

crab::Ref<crab::D11_Texture1D> D11_Texture::Create1D(const D3D11_TEXTURE1D_DESC& in_desc)
{
    Ref<D11_Texture1D> tex = CreateRef<D11_Texture1D>();
    auto               d   = D11_API->GetDevice();

    D11_ASSERT(d->CreateTexture1D(&in_desc, nullptr, tex->texture.GetAddressOf()), "CreateTexture1D Fail.");
    tex->desc = in_desc;

    return tex;
}

crab::Ref<crab::D11_Texture2D> D11_Texture::Create2D(const D3D11_TEXTURE2D_DESC& in_desc)
{
    Ref<D11_Texture2D> tex = CreateRef<D11_Texture2D>();
    auto               d   = D11_API->GetDevice();

    D11_ASSERT(d->CreateTexture2D(&in_desc, nullptr, tex->texture.GetAddressOf()), "CreateTexture2D Fail.");
    tex->desc = in_desc;

    return tex;
}

crab::Ref<crab::D11_Texture3D> D11_Texture::Create3D(const D3D11_TEXTURE3D_DESC& in_desc)
{
    Ref<D11_Texture3D> tex = CreateRef<D11_Texture3D>();
    auto               d   = D11_API->GetDevice();

    D11_ASSERT(d->CreateTexture3D(&in_desc, nullptr, tex->texture.GetAddressOf()), "CreateTexture3D Fail.");
    tex->desc = in_desc;

    return tex;
}

crab::Ref<crab::D11_Texture1D> D11_Texture::Create1DFromFile(const std::filesystem::path& in_path)
{
    Ref<D11_Texture1D> tex = _CreateFromFileHelper<D11_Texture1D>(in_path);
    return tex;
}


crab::Ref<crab::D11_Texture2D> D11_Texture::Create2DFromFile(const std::filesystem::path& in_path)
{
    Ref<D11_Texture2D> tex = _CreateFromFileHelper<D11_Texture2D>(in_path);
    return tex;
}

crab::Ref<crab::D11_Texture3D> D11_Texture::Create3DFromFile(const std::filesystem::path& in_path)
{
    Ref<D11_Texture3D> tex = _CreateFromFileHelper<D11_Texture3D>(in_path);
    return tex;
}

}   // namespace crab
