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
                                   const D3D11_SHADER_RESOURCE_VIEW_DESC& in_desc);

    static Ref<D11_Texture> Create(const std::filesystem::path& in_path);

    static Ref<D11_Texture> CreateTextureCube(
        const std::filesystem::path& in_pathPX,
        const std::filesystem::path& in_pathNX,
        const std::filesystem::path& in_pathPY,
        const std::filesystem::path& in_pathNY,
        const std::filesystem::path& in_pathPZ,
        const std::filesystem::path& in_pathNZ);

    static Ref<D11_Texture> CreateTextureCubeByDDS(const std::filesystem::path& in_path);

    static Ref<D11_Texture> CreateTexture2DArray(const std::vector<std::filesystem::path>& in_paths);

    void Bind(uint32 in_slot, eShaderFlags in_flag);

    ID3D11ShaderResourceView* Get() const;
    DirectX::TexMetadata      GetImageData() const { return m_imageData; }

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;
    DirectX::TexMetadata             m_imageData;
};

}   // namespace crab