#pragma once

namespace crab
{

//======================================
// my renderer engine use DirectXTex Lib
// to handle texture
//======================================

struct TextureLoadData
{
    bool succeed;

    std::filesystem::path path;
    DirectX::TexMetadata  metadata;
    DirectX::ScratchImage scratchImage;
};

class TextureLoader
{
public:
    static TextureLoadData LoadFromFile(
        const std::filesystem::path& in_path,
        eTextureLoadFlags            in_flags);

    static void GenerateMipmap(TextureLoadData& inout_data);
    static void ReverseToneMap(TextureLoadData& inout_data);
    static void GenerateCubeMap(TextureLoadData& inout_data);
    static void DisableMipmap(TextureLoadData& inout_data);
};

}