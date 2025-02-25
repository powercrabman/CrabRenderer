#pragma once
#include "D11Utils.h"

namespace crab
{

//===================================================
//                    Image
//===================================================

// this is wrapper of ID3D11ShaderResourceView
class Image2D
{
    D11_RESOURCE_CTOR(Image2D);

public:
    //===================================================
    //                    Factory
    //===================================================
    static Ref<Image2D> CreateFromTexture(ID3D11Texture2D* in_texture);
    static Ref<Image2D> CreateFromTexture(ID3D11Texture2D* in_texture, eFormat in_format);
    static Ref<Image2D> CreateFromBuffer(ID3D11Buffer* in_buffer);
    static Ref<Image2D> CreateFromBuffer(ID3D11Buffer* in_buffer, eFormat in_format);

    // if in_mipMapLevelCount = 0 -> No mipmap
    static Ref<Image2D> CreateFromFile(const std::filesystem::path& in_path, bool in_enableMipMap = true);
    static Ref<Image2D> CreateTextureArrayFromFile(const std::vector<std::filesystem::path>& in_paths, bool in_enableMipMap = true);
    static Ref<Image2D> CreateTextureArrayFromFile(const std::filesystem::path& in_path, uint32 in_arrayMaxCount, bool in_enableMipMap = true);

    //===================================================
    //                 Member Function
    //===================================================

    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11ShaderResourceView* GetSRV() const;

    auto    GetTextureSize() const { return std::make_pair(m_width, m_height); }
    eFormat GetFormat() const { return m_format; }
    uint32  GetArraySize() const { return m_arraySize; }
    uint32  GetMipMapLevelCount() const { return m_mipMapLevelCount; }

    bool IsTextureCube() const;

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;

    uint32 m_width  = 0;
    uint32 m_height = 0;

    uint32 m_arraySize        = 1;
    uint32 m_mipMapLevelCount = 1;

    eFormat m_format = eFormat::Unknown;
};

//===================================================
// Staging Texture
//===================================================

class StagingTexture2D
{
    D11_RESOURCE_CTOR(StagingTexture2D);

public:
    static Ref<StagingTexture2D> Create(
        uint32  in_width,
        uint32  in_height,
        eFormat in_format,
        bool    in_cpuRead,
        bool    in_cpuWrite);

    void WriteToTexture(
        void*  in_data,
        uint32 in_pixelStride,
        uint32 in_width,
        uint32 in_height) const;

    void ReadFromTexture(void* out_data) const;

    void CopyTo(ID3D11Texture2D* in_destTexture) const;
    void CopyFrom(ID3D11Texture2D* in_srcTexture) const;

    auto    TextureSize() const { return std::make_pair(m_width, m_height); }
    eFormat GetFormat() const { return m_format; }

private:
    ComPtr<ID3D11Texture2D> m_texture;

    uint32 m_width  = 0;
    uint32 m_height = 0;

    eFormat m_format = eFormat::Unknown;
};

//===================================================
// CubemapImage
//===================================================

class CubemapImage
{
    D11_RESOURCE_CTOR(CubemapImage);

public:
    static Ref<CubemapImage> Create(const std::filesystem::path& in_path);

    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    Ref<Image2D> GetImage() const { return m_image; }

private:
    Ref<Image2D> m_image;
};

}   // namespace crab
