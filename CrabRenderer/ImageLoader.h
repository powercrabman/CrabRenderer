#pragma once

namespace crab
{

//======================================
// my renderer engine use DirectXTex Lib
// to handle texture
//======================================

struct ImageLoaderData
{
    bool succeed;

    std::filesystem::path path;
    DirectX::TexMetadata  metadata;
    DirectX::ScratchImage scratchImage;
};

class ImageLoader
{
public:
    static ImageLoaderData LoadFromFile(const std::filesystem::path& in_path);
    static ImageLoaderData LoadTextureCubeFromFile(const std::filesystem::path& in_path);

    static void GenerateMipmap(ImageLoaderData& inout_data);
};

}