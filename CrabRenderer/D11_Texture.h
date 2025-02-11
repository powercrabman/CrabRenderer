#pragma once
#include "D11_Utiles.h"

namespace crab
{

//===================================================
//                     Utile
//===================================================

struct D11_TextureUtile
{
    static DirectX::ScratchImage LoadImageFromFile(const std::filesystem::path& in_path);
};

//===================================================
//                    Texture2D
//===================================================

class D11_Texture2D
{
public:
    //===================================================
    //                 Basic Texture 2D
    //===================================================

    static Ref<D11_Texture2D> Create(ID3D11Texture2D*                       in_texture,
                                     const D3D11_SHADER_RESOURCE_VIEW_DESC& in_desc);

    static Ref<D11_Texture2D> Create(const std::filesystem::path& in_path);
    static Ref<D11_Texture2D> CreateMipmap(const std::filesystem::path& in_path, uint32 in_mipLevel = 0);

    //===================================================
    //                  Texture Array
    //===================================================

    static Ref<D11_Texture2D> CreateTextureArray(const std::vector<std::filesystem::path>& in_paths);

    //===================================================
    //                    Cube map
    //===================================================

    static Ref<D11_Texture2D> CreateTextureCube(
        const std::filesystem::path& in_pathPX,
        const std::filesystem::path& in_pathNX,
        const std::filesystem::path& in_pathPY,
        const std::filesystem::path& in_pathNY,
        const std::filesystem::path& in_pathPZ,
        const std::filesystem::path& in_pathNZ);

    static Ref<D11_Texture2D> CreateTextureCubeByDDS(const std::filesystem::path& in_path);

    //===================================================
    //                 Member Function
    //===================================================

    void Bind(uint32 in_slot, eShaderFlags in_flag);

    ID3D11ShaderResourceView* GetSRV() const;
    DirectX::Image            GetImageData() const { return m_image; }

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;
    DirectX::Image                   m_image;
};

//===================================================
//                  Texture List
//===================================================

class D11_TextureList
{
public:
    D11_TextureList()  = default;
    ~D11_TextureList() = default;

    void             ClearList();
    D11_TextureList& Add(Ref<D11_Texture2D> in_texture, uint32 in_slot, eShaderFlags in_flag);

    Ref<D11_Texture2D> GetTexture(uint32 in_index) const;

    void Bind();

private:
    struct TextureNode
    {
        Ref<D11_Texture2D> texture;
        uint32             slot;
        eShaderFlags       flag;
    };

    std::vector<TextureNode> m_nodes;
};

}   // namespace crab
