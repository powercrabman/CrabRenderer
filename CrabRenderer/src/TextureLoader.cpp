#include "InternalPCH.h"


#include "TextureLoader.h"

#include "Renderer.h"
#include "SwapChain.h"

#include <DirectXTex.h>
#include <DirectXTexEXR.h>

namespace crab
{

TextureLoadData TextureLoader::LoadFromFile(
    const std::filesystem::path& in_path,
    eTextureLoadFlags            in_flags)
{
    using namespace DirectX;
    const std::filesystem::path ext = in_path.extension();

    TextureLoadData loadData = {};
    loadData.path            = in_path;

    log::Info("Load Image: {0}", in_path.string());

    if (ext == ".dds")
    {
        loadData.succeed = d3d11::CheckOK(LoadFromDDSFile(
                                            in_path.c_str(),
                                            DDS_FLAGS_NONE,
                                            &loadData.metadata,
                                            loadData.scratchImage),
                                        "LoadFromDDSFile Fail.");
    }
    else if (ext == ".exr")
    {
        loadData.succeed = d3d11::CheckOK(LoadFromEXRFile(
                                            in_path.c_str(),
                                            &loadData.metadata,
                                            loadData.scratchImage),
                                        "LoadFromEXRFile Fail.");
    }
    else
    {
        WIC_FLAGS flags = WIC_FLAGS_NONE;

        loadData.succeed = d3d11::CheckOK(LoadFromWICFile(
                                            in_path.c_str(),
                                            flags,
                                            &loadData.metadata,
                                            loadData.scratchImage),
                                        "LoadFromWICFile Fail.");
    }

    if (loadData.succeed)
    {
        // Processing

        if (in_flags & eTextureLoadFlags_DisableMipmaps)
        {
            DisableMipmap(loadData);
        }
        else
        {
            if (in_flags & eTextureLoadFlags_GenerateMips)
                GenerateMipmap(loadData);
        }

        if (in_flags & eTextureLoadFlags_ReverseTone)
            ReverseToneMap(loadData);

        if (in_flags & eTextureLoadFlags_GenerateCubeMap)
            GenerateCubeMap(loadData);
    }
    else
    {
        log::Error("Load Image Fail: {0}", in_path.string());
    }

    return loadData;
}

void TextureLoader::GenerateMipmap(TextureLoadData& inout_data)
{
    using namespace DirectX;

    if (inout_data.succeed)
    {
        ScratchImage mipChain;
        if (d3d11::CheckOK(GenerateMipMaps(
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

void TextureLoader::ReverseToneMap(TextureLoadData& inout_data)
{
    using namespace DirectX;

    if (inout_data.succeed)
    {
        ScratchImage toneMap;
        if (d3d11::CheckOK(Convert(
                             inout_data.scratchImage.GetImages(),
                             inout_data.scratchImage.GetImageCount(),
                             inout_data.metadata,
                             DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
                             TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT,
                             0.5f,
                             toneMap),
                         "Convert Fail."))
        {
            inout_data.scratchImage = std::move(toneMap);
            inout_data.metadata     = inout_data.scratchImage.GetMetadata();
        }
    }
}

void TextureLoader::GenerateCubeMap(TextureLoadData& inout_data)
{
    using namespace DirectX;

    if (!inout_data.succeed)
        return;

    if (inout_data.metadata.IsCubemap())
        return;

    const Image* images     = inout_data.scratchImage.GetImages();
    size_t       imageCount = inout_data.scratchImage.GetImageCount();

    if (imageCount < 6)
    {
        log::Error("GenerateCubeMap Fail: Not enough images for cubemap!");
        return;
    }

    ScratchImage cubeMap;

    if (d3d11::CheckOK(cubeMap.InitializeCubeFromImages(images, 6),
                     "InitializeCubeFromImages Fail."))
    {
        inout_data.scratchImage = std::move(cubeMap);
        inout_data.metadata     = inout_data.scratchImage.GetMetadata();
    }
}

void TextureLoader::DisableMipmap(TextureLoadData& inout_data)
{
    if (inout_data.succeed)
        inout_data.metadata.mipLevels = 1;
}

}   // namespace crab
