#pragma once
#include "CrabEnums.h"
#include "D11Utils.h"

namespace crab
{

//===================================================
// Texture
//===================================================

class Texture
{
public:
    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11ShaderResourceView* GetSRV() const;
    auto                      GetTextureSize() const { return std::make_pair(m_width, m_height); }
    eFormat                   GetFormat() const { return m_format; }
    uint32                    GetMipMapLevelCount() const { return m_mipMapLevelCount; }
    eTextureType              GetTextureType() const { return m_textureType; }

protected:
    Texture() = default;

    ComPtr<ID3D11ShaderResourceView> m_srv;

    uint32 m_width  = 0;
    uint32 m_height = 0;

    uint32 m_mipMapLevelCount = 1;

    eFormat      m_format      = eFormat::Unknown;
    eTextureType m_textureType = eTextureType::Unknown;
};

template<typename T>
inline Ref<T> CastTexture(const Ref<Texture>& in_texture)
{
    static_assert(IS_BASE_OF(Texture, T), "TextureCast: T must be derived from Texture.");
    if constexpr (T::s_staticType == eTextureType::Texture2D)
        return std::static_pointer_cast<T>(in_texture);
    else if constexpr (T::s_staticType == eTextureType::TextureCube)
        return std::static_pointer_cast<T>(in_texture);
    else if constexpr (T::s_staticType == eTextureType::Texture2DArray)
        return std::static_pointer_cast<T>(in_texture);
    else
        static_assert(false, "TextureCast: Unknown Texture Type.");

    return nullptr;
}

//===================================================
//                   Texture2D
//===================================================

class Texture2D : public Texture
{

public:
    void Init(ID3D11Texture2D* in_texture, eFormat in_format = eFormat::Unknown);
    void Init(ID3D11Buffer* in_buffer, eFormat in_format = eFormat::Unknown);

    void LoadFromFile(const std::filesystem::path& in_path,
                      bool                         in_generateMips       = true,
                      bool                         in_inverseToneMapping = false);

    inline static eTextureType s_staticType = eTextureType::Texture2D;
};

//===================================================
// Texture Cube
//===================================================

class TextureCube : public Texture
{
public:
    void Init(
        ID3D11Texture2D* in_texture,
        eFormat          in_format = eFormat::Unknown);

    void LoadFromFile(
        const std::filesystem::path& in_path,
        bool                         in_generateMips = true);

    inline static eTextureType s_staticType = eTextureType::TextureCube;
};

//===================================================
// Texture Array
//===================================================

class Texture2DArray : public Texture
{

public:
    // Factory
    void LoadFromFile(
        const std::vector<std::filesystem::path>& in_paths,
        bool                                      in_generateMips       = true,
        bool                                      in_inverseToneMapping = false);

    void LoadFromFile(
        const std::filesystem::path& in_path,
        uint32                       in_arrayMaxCount,
        bool                         in_generateMips       = true,
        bool                         in_inverseToneMapping = false);

    void Init(const std::vector<Ref<Texture2D>>& in_textures);
    void Init(const std::vector<ID3D11Texture2D*>& in_textures);
    void Init(ID3D11Texture2D* in_textures,
              eFormat          in_textureFormat = eFormat::Unknown);

    uint32                     GetArraySize() const { return m_arraySize; }
    inline static eTextureType s_staticType = eTextureType::Texture2DArray;

private:
    uint32 m_arraySize = 0;
};

//===================================================
// Staging Texture
//===================================================

class D11StagingTexture
{
public:
    void Init(
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

    uint32  m_width  = 0;
    uint32  m_height = 0;
    eFormat m_format = eFormat::Unknown;
};

}   // namespace crab
