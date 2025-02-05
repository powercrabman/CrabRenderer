#pragma once
#include "D11_Utiles.h"

namespace crab
{

struct D11_SamplerState;

struct Texture
{
    // - Factory
    static Ref<Texture> Create(const std::filesystem::path& in_path,
                               const Ref<D11_SamplerState>& in_samplerState);

    static Ref<Texture> CreateTextureCube(
        const std::filesystem::path& in_pathPX,
        const std::filesystem::path& in_pathNX,
        const std::filesystem::path& in_pathPY,
        const std::filesystem::path& in_pathNY,
        const std::filesystem::path& in_pathPZ,
        const std::filesystem::path& in_pathNZ,
        const Ref<D11_SamplerState>& in_samplerState);

    static Ref<Texture> CreateTextureCubeByDDS(
        const std::filesystem::path& in_path,
        const Ref<D11_SamplerState>& in_samplerState);

    ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::TexMetadata             imageData;
    Ref<D11_SamplerState>            samplerState;   // move to material
};

struct TextureArray
{
    TextureArray() = default;
    TextureArray(const std::vector<Ref<Texture>>& in_textures);
    Ref<Texture>&       operator[](size_t in_index) { return textures[in_index]; }
    const Ref<Texture>& operator[](size_t in_index) const { return textures[in_index]; }

    TextureArray& Add(const Ref<Texture>& in_texture);
    void Clear();

    std::vector<Ref<Texture>>              textures;
    std::vector<ID3D11ShaderResourceView*> srvPtrs;
    std::vector<ID3D11SamplerState*>       statePtrs;
};

}   // namespace crab