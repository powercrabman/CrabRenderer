#include "CrabPch.h"

#include "ImageLoader.h"

#include "D11Renderer.h"
#include "Swapchain.h"

#include <DirectXTex.h>
#include <DirectXTexEXR.h>

namespace crab
{

ImageLoaderData ImageLoader::LoadFromFile(const std::filesystem::path& in_path)
{
    using namespace DirectX;
    const std::filesystem::path ext = in_path.extension();

    ImageLoaderData loadData = {};
    loadData.path            = in_path;

    Log::Info("Load Image: {0}", in_path.string());

    if (ext == ".dds")
    {
        loadData.succeed = CheckD3D11Result(LoadFromDDSFile(
                                              in_path.c_str(),
                                              DDS_FLAGS_NONE,
                                              &loadData.metadata,
                                              loadData.scratchImage),
                                          "LoadFromDDSFile Fail.");
    }
    else if (ext == ".exr")
    {
        loadData.succeed = CheckD3D11Result(LoadFromEXRFile(
                                              in_path.c_str(),
                                              &loadData.metadata,
                                              loadData.scratchImage),
                                          "LoadFromEXRFile Fail.");
    }
    else
    {
        WIC_FLAGS flags = WIC_FLAGS_NONE;

        if (GetRenderer().GetSwapChain()->IsHDR())
            flags = WIC_FLAGS_DEFAULT_SRGB;

        loadData.succeed = CheckD3D11Result(LoadFromWICFile(
                                              in_path.c_str(),
                                              flags,
                                              &loadData.metadata,
                                              loadData.scratchImage),
                                          "LoadFromWICFile Fail.");
    }

    return loadData;
}

ImageLoaderData ImageLoader::LoadTextureCubeFromFile(const std::filesystem::path& in_path)
{
    using namespace DirectX;
    const std::filesystem::path ext = in_path.extension();

    ImageLoaderData loadData = {};
    loadData.path            = in_path;

    if (ext == ".dds")
    {
        loadData.succeed = CheckD3D11Result(LoadFromDDSFile(
                                              in_path.c_str(),
                                              DDS_FLAGS_NONE,
                                              &loadData.metadata,
                                              loadData.scratchImage),
                                          "LoadFromDDSFile Fail.");
    }
    else if (ext == ".exr")
    {
        loadData.succeed = CheckD3D11Result(LoadFromEXRFile(
                                              in_path.c_str(),
                                              &loadData.metadata,
                                              loadData.scratchImage),
                                          "LoadFromEXRFile Fail.");
    }
    else
    {
        CRAB_DEBUG_BREAK("WIC not support cubemap.");
    }

    if (loadData.succeed && loadData.metadata.IsCubemap())
    {
        loadData.succeed = true;
    }
    else
    {
        loadData         = {};
        loadData.succeed = false;
    }

    return loadData;
}

void ImageLoader::GenerateMipmap(ImageLoaderData& inout_data)
{
    using namespace DirectX;

    if (inout_data.succeed)
    {
        ScratchImage mipChain;
        if (CheckD3D11Result(GenerateMipMaps(
                               inout_data.scratchImage.GetImages(),
                               inout_data.scratchImage.GetImageCount(),
                               inout_data.metadata,
                               TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT,
                               0,
                               mipChain),
                           "GenerateMipMaps Fail."))
        {
            inout_data.scratchImage = std::move(mipChain);
            inout_data.metadata     = inout_data.scratchImage.GetMetadata();
        }
    }
}

}   // namespace crab
