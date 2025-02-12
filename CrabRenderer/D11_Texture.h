#pragma once
#include "D11_Utils.h"

namespace crab
{

//===================================================
//                     Utils
//===================================================

struct D11_TextureLoader
{
    static bool LoadImageFromFile(const std::filesystem::path& in_path,
                                  DirectX::ScratchImage*       out_scratchImage,
                                  DirectX::TexMetadata*        out_metaData);
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
    static Ref<D11_Texture2D> CreateTextureCube(const std::filesystem::path& in_path);

    //===================================================
    //                  Texture Array
    //===================================================

    static Ref<D11_Texture2D> CreateTextureArray(const std::vector<std::filesystem::path>& in_paths);
    static Ref<D11_Texture2D> CreateMipmapTextureArray(const std::vector<std::filesystem::path>& in_paths, uint32 in_mipLevel = 0);
    static Ref<D11_Texture2D> CreateTextureCubeArray(const std::vector<std::filesystem::path>& in_paths);

    //===================================================
    //                 Member Function
    //===================================================

    void Bind(uint32 in_slot, eShaderFlags in_flag) const;

    ID3D11ShaderResourceView* GetSRV() const;
    DirectX::Image            GetImageData() const { return m_image; }

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;
    DirectX::Image                   m_image = {};
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

    void Bind() const;

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
