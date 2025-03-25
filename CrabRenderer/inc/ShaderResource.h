#pragma once

namespace crab
{

//===================================================
// ShaderResource
//===================================================

class ShaderResource
{
public:
    void Bind(uint32 in_slot, eShaderFlags in_bindFlags) const;

    ID3D11ShaderResourceView* GetSRV() const;
    eResourceType             GetResourceType() const { return m_resourceType; }

protected:
    ShaderResource() = default;

    ComPtr<ID3D11ShaderResourceView> m_srv;

    eResourceType m_resourceType = eResourceType::Unknown;
};

//===================================================
// Buffer
//===================================================

class BufferShaderResource : public ShaderResource
{
public:
    void Init(ID3D11Buffer* in_buffer);

    uint32        GetByteWidth() const { return m_byteWidth; }
    ID3D11Buffer* GetBuffer() const { return m_buffer.Get(); }

    inline static eResourceType s_staticType = eResourceType::Buffer;

private:
    ComPtr<ID3D11Buffer> m_buffer;
    uint32               m_byteWidth = 0;
};

//===================================================
// ShaderResource Texture
//===================================================

class TextureShaderResource : public ShaderResource
{
public:
    auto             GetTextureSize() const { return std::make_pair(m_width, m_height); }
    uint32           GetMipMapLevelCount() const { return m_mipMapLevelCount; }
    eFormat          GetFormat() const { return m_format; }
    ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }

protected:
    TextureShaderResource() = default;

    ComPtr<ID3D11Texture2D> m_texture;

    uint32 m_width  = 0;
    uint32 m_height = 0;

    uint32  m_mipMapLevelCount = 1;
    eFormat m_format           = eFormat::Unknown;
};

//===================================================
// Texture2D
//===================================================

class Texture2D : public TextureShaderResource
{
public:
    void Init(ID3D11Texture2D* in_texture, eFormat in_format);

    void LoadFromFile(const std::filesystem::path& in_path,
                      eTextureLoadFlags            in_flags);

    inline static eResourceType s_staticType = eResourceType::Texture2D;
};

//===================================================
// ShaderResource Cube
//===================================================

class TextureCube : public TextureShaderResource
{
public:
    void Init(const std::array<ID3D11Texture2D*, 6>& in_textures, eFormat in_format);
    void Init(ID3D11Texture2D* in_textureCube, eFormat in_format);

    void LoadFromFile(const std::filesystem::path& in_path,
                      eTextureLoadFlags            in_flags);

    inline static eResourceType s_staticType = eResourceType::TextureCube;
};

//===================================================
// ShaderResource Array
//===================================================

class Texture2DArray : public TextureShaderResource
{

public:
    void Init(ID3D11Texture2D* in_textureArray, eFormat in_format);
    void Init(const std::vector<ID3D11Texture2D*>& in_textures, eFormat in_format);

    uint32 GetArraySize() const { return m_arraySize; }

    inline static eResourceType s_staticType = eResourceType::Texture2DArray;

private:
    void InitMemberValues_Internal(const D3D11_TEXTURE2D_DESC& in_desc);

    uint32 m_arraySize = 0;
};

//===================================================
// Staging ShaderResource
//===================================================

class StagingTexture
{
public:
    void Init(uint32  in_width,
              uint32  in_height,
              eFormat in_format,
              bool    in_cpuRead,
              bool    in_cpuWrite);

    void WriteToTexture(const void* in_writeData, uint32 in_writeDataByteWidth) const;
    void ReadFromTexture(void* out_data, OPTIONAL uint32* out_dataByteWidth) const;

    void CopyTo(ID3D11Texture2D* in_destTexture) const;
    void CopyFrom(ID3D11Texture2D* in_srcTexture) const;

    auto             TextureSize() const { return std::make_pair(m_width, m_height); }
    eFormat          GetFormat() const { return m_format; }
    ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }

private:
    ComPtr<ID3D11Texture2D> m_texture;

    uint32  m_width  = 0;
    uint32  m_height = 0;
    eFormat m_format = eFormat::Unknown;
};

}   // namespace crab
