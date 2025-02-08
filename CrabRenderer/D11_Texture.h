#pragma once
#include "D11_Utiles.h"

namespace crab
{

class D11_SamplerState;

class D11_Texture
{
public:
    // - Factory
    static Ref<D11_Texture> Create(ID3D11Texture2D*                       in_texture,
                                   const D3D11_SHADER_RESOURCE_VIEW_DESC& in_desc,
                                   const Ref<D11_SamplerState>&           in_samplerState);

    static Ref<D11_Texture> Create(const std::filesystem::path& in_path,
                                   const Ref<D11_SamplerState>& in_samplerState);

    static Ref<D11_Texture> CreateTextureCube(
        const std::filesystem::path& in_pathPX,
        const std::filesystem::path& in_pathNX,
        const std::filesystem::path& in_pathPY,
        const std::filesystem::path& in_pathNY,
        const std::filesystem::path& in_pathPZ,
        const std::filesystem::path& in_pathNZ,
        const Ref<D11_SamplerState>& in_samplerState);

    static Ref<D11_Texture> CreateTextureCubeByDDS(
        const std::filesystem::path& in_path,
        const Ref<D11_SamplerState>& in_samplerState);

    void Bind(uint32 in_slot, eShaderFlags in_flag);

    ID3D11ShaderResourceView* Get() const;
    ID3D11SamplerState*       GetSamplerState() const;
    DirectX::TexMetadata      GetMetaData() const { return m_imageData; }

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;
    Ref<D11_SamplerState>            m_samplerState;
    DirectX::TexMetadata             m_imageData;
};

class D11_TextureArray
{
public:
    D11_TextureArray() = default;
    D11_TextureArray(const std::vector<Ref<D11_Texture>>& in_textures);
    ~D11_TextureArray() = default;

    void Bind(uint32 in_startSlot, eShaderFlags in_flag) const;
    void Add(const Ref<D11_Texture>& in_texture);

private:
    std::vector<Ref<D11_Texture>>          m_textures;
    std::vector<ID3D11ShaderResourceView*> m_srvs;
    std::vector<ID3D11SamplerState*>       m_samplerStates;
};

}   // namespace crab