#include "CrabPch.h"

#include "Snapshot.h"

#include "RenderTarget.h"
#include "D11Renderer.h"
#include "Textures.h"

namespace crab
{

Snapshot::Snapshot()
{
}

Snapshot::~Snapshot()
{
}

void Snapshot::Capture(const Ref<RenderTarget>& in_rt)
{
    auto dx  = GetRenderer().GetDevice();
    auto ctx = GetRenderer().GetContext();

    ComPtr<ID3D11Resource> res;
    in_rt->Get()->GetResource(res.GetAddressOf());

    ComPtr<ID3D11Texture2D> tex;
    CheckD3D11Result(res.As(&tex), "As Fail.");

    D3D11_TEXTURE2D_DESC desc = {};
    tex->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC captureDesc = desc;
    captureDesc.BindFlags            = 0;
    captureDesc.CPUAccessFlags       = D3D11_CPU_ACCESS_READ;
    captureDesc.Usage                = D3D11_USAGE_STAGING;
    m_format                         = desc.Format;

    CheckD3D11Result(dx->CreateTexture2D(
                   &captureDesc,
                   nullptr,
                   m_texture.GetAddressOf()),
               "CreateTexture2D Fail.");

    ctx->CopyResource(m_texture.Get(), tex.Get());
    m_width  = desc.Width;
    m_height = desc.Height;
    Log::Info("Snapshot captured: {0}x{1}", m_width, m_height);
}

void Snapshot::Save(const std::filesystem::path& in_path)
{
    if (m_texture == nullptr)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return;
    }

    auto dx  = GetRenderer().GetDevice();
    auto ctx = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    CheckD3D11Result(ctx->Map(m_texture.Get(), 0, D3D11_MAP_READ, 0, &mapped), "Map Fail.");

    DirectX::ScratchImage image;

    D3D11_TEXTURE2D_DESC desc = {};
    m_texture->GetDesc(&desc);
    CheckD3D11Result(image.Initialize2D(m_format,
                                  desc.Width,
                                  desc.Height,
                                  1,
                                  1),
               "Initialize2D Fail.");

    uint8_t* dest       = image.GetPixels();
    uint8_t* src        = (uint8_t*)mapped.pData;
    size_t   rowPitch   = mapped.RowPitch;
    size_t   slicePitch = rowPitch * desc.Height;

    for (size_t i = 0; i < desc.Height; i++)
    {
        memcpy(dest, src, rowPitch);
        dest += rowPitch;
        src += rowPitch;
    }

    ctx->Unmap(m_texture.Get(), 0);

    std::filesystem::path fileName = in_path.stem();
    std::filesystem::path ext      = in_path.extension();

    DirectX::WICCodecs codec = {};

    if (ext == ".jpg" || ext == ".jpeg")
        codec = DirectX::WIC_CODEC_JPEG;
    else if (ext == ".bmp")
        codec = DirectX::WIC_CODEC_BMP;
    else if (ext == ".tiff")
        codec = DirectX::WIC_CODEC_TIFF;
    else if (ext == ".gif")
        codec = DirectX::WIC_CODEC_GIF;
    else if (ext == ".png" || ext.empty())
        codec = DirectX::WIC_CODEC_PNG;
    else
    {
        CRAB_DEBUG_BREAK("Invalid extension.");
        return;
    }

    CheckD3D11Result(DirectX::SaveToWICFile(image.GetImages(),
                                      image.GetImageCount(),
                                      DirectX::WIC_FLAGS_NONE,
                                      DirectX::GetWICCodec(codec),
                                      in_path.c_str()),
               "SaveToWICFile Fail.");

    Log::Info("Snapshot saved: {0}", in_path.string());
}

Vec4 Snapshot::GetPixelColorFloat(int in_x, int in_y)
{
    if (m_texture == nullptr)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return Vec4::Zero;
    }

    if (in_x < 0 || in_y < 0)
    {
        CRAB_DEBUG_BREAK("Invalid x, y.");
        return Vec4::Zero;
    }

    auto ctx = GetRenderer().GetContext();

    D3D11_MAPPED_SUBRESOURCE mapped = {};
    CheckD3D11Result(ctx->Map(m_texture.Get(),
                        0,
                        D3D11_MAP_READ,
                        0,
                        &mapped),
               "Map Fail.");

    uint8_t* src      = (uint8_t*)mapped.pData;
    size_t   rowPitch = mapped.RowPitch;

    uint8_t* pixel = src + in_y * rowPitch + in_x * 4;

    Vec4 color;
    color.x = pixel[0] / 255.f;
    color.y = pixel[1] / 255.f;
    color.z = pixel[2] / 255.f;
    color.w = pixel[3] / 255.f;

    ctx->Unmap(m_texture.Get(), 0);

    return color;
}

crab::Vec4Int Snapshot::GetPixelColorUInt(int in_x, int in_y)
{
    if (m_texture == nullptr)
    {
        CRAB_DEBUG_BREAK("Texture is nullptr.");
        return Vec4Int { 0, 0, 0, 0 };
    }
    if (in_x < 0 || in_y < 0)
    {
        CRAB_DEBUG_BREAK("Invalid x, y.");
        return Vec4Int { 0, 0, 0, 0 };
    }
    auto                     ctx    = GetRenderer().GetContext();
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    CheckD3D11Result(ctx->Map(m_texture.Get(),
                        0,
                        D3D11_MAP_READ,
                        0,
                        &mapped),
               "Map Fail.");
    uint8_t* src      = (uint8_t*)mapped.pData;
    size_t   rowPitch = mapped.RowPitch;
    uint8_t* pixel    = src + in_y * rowPitch + in_x * 4;
    Vec4Int color;
    color.x = pixel[0];
    color.y = pixel[1];
    color.z = pixel[2];
    color.w = pixel[3];
    ctx->Unmap(m_texture.Get(), 0);
    return color;
}

}   // namespace crab