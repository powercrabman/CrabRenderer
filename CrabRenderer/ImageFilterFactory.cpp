#include "CrabPch.h"

#include "ImageFilterFactory.h"

#include "D11Renderer.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "ImageFilter.h"
#include "RenderFactory.h"
#include "Textures.h"

namespace crab
{

Ref<ImageFilter> ImageFilterFactory::CreateBlurDown(
    uint32                                 in_width,
    uint32                                 in_height,
    const Ref<Texture2D>&                  in_inputTexture,
    Ref<ConstantBuffer<BlurDownConstant>>* out_constantBuffer)
{
    TextureList  textures;
    SamplerList  samplers;
    ConstantList constants;

    textures.Init({ TextureNode { in_inputTexture, 0, eShaderFlags_PixelShader } });
    samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader } });

    auto cb = CreateConstantBuffer(BlurDownConstant {.dx = 1.f / in_width, .dy = 1.f / in_height});
    constants.Init({ ConstantNode { cb, 0, eShaderFlags_PixelShader } });

    if (out_constantBuffer)
        *out_constantBuffer = cb;

    Ref<ImageFilter> filter = CreateRef<ImageFilter>();
    filter->Init(
        in_width,
        in_height,
        GetGlobalShader()->GetScreenSpaceVS(),
        GetGlobalShader()->GetBlurDownPS(),
        textures,
        samplers,
        constants);

    return filter;
}

Ref<ImageFilter> ImageFilterFactory::CreateBlurUp(
    uint32                               in_width,
    uint32                               in_height,
    const Ref<Texture2D>&                in_inputTexture,
    const BlurUpConstant&                in_initData,
    Ref<ConstantBuffer<BlurUpConstant>>* out_constantBuffer)
{
    TextureList  textures;
    SamplerList  samplers;
    ConstantList constants;

    textures.Init({ TextureNode { in_inputTexture, 0, eShaderFlags_PixelShader } });
    samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader } });

    auto cb = CreateConstantBuffer(in_initData);
    constants.Init({ ConstantNode { cb, 0, eShaderFlags_PixelShader } });

    if (out_constantBuffer)
        *out_constantBuffer = cb;

    Ref<ImageFilter> filter = CreateRef<ImageFilter>();
    filter->Init(
        in_width,
        in_height,
        GetGlobalShader()->GetScreenSpaceVS(),
        GetGlobalShader()->GetBlurUpPS(),
        textures,
        samplers,
        constants);

    return filter;
}

Ref<ImageFilter> ImageFilterFactory::CreateSampling(
    uint32                in_width,
    uint32                in_height,
    const Ref<Texture2D>& in_inputTexture)
{

    TextureList  textures;
    SamplerList  samplers;
    ConstantList constants;

    textures.Init({ TextureNode { in_inputTexture, 0, eShaderFlags_PixelShader } });
    samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader } });

    Ref<ImageFilter> filter = CreateRef<ImageFilter>();
    filter->Init(
        in_width,
        in_height,
        GetGlobalShader()->GetScreenSpaceVS(),
        GetGlobalShader()->GetSamplingPS(),
        textures,
        samplers,
        constants);

    return filter;
}

Ref<ImageFilter> ImageFilterFactory::CreateToneMapping(
    uint32                                    in_width,
    uint32                                    in_height,
    const Ref<Texture2D>&                     in_inputTexture,
    const ToneMappingConstant&                in_initData,
    Ref<ConstantBuffer<ToneMappingConstant>>* out_constantBuffer)
{
    TextureList  textures;
    SamplerList  samplers;
    ConstantList constants;

    textures.Init({ TextureNode { in_inputTexture, 0, eShaderFlags_PixelShader } });
    samplers.Init({ SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader } });

    auto cb = CreateConstantBuffer(in_initData);
    constants.Init({ ConstantNode { cb, 0, eShaderFlags_PixelShader } });

    if (out_constantBuffer)
        *out_constantBuffer = cb;

    Ref<ImageFilter> filter = CreateRef<ImageFilter>();
    filter->Init(
        in_width,
        in_height,
        GetGlobalShader()->GetScreenSpaceVS(),
        GetGlobalShader()->GetToneMapPS(),
        textures,
        samplers,
        constants);

    return filter;
}

Ref<ImageFilter> ImageFilterFactory::CreateCombine(
    uint32                                in_width,
    uint32                                in_height,
    const Ref<Texture2D>&                 in_inputSrcTexture,
    const Ref<Texture2D>&                 in_inputDstTexture,
    const CombineConstant&                in_initData,
    Ref<ConstantBuffer<CombineConstant>>* out_constantBuffer)
{
    TextureList  textures;
    SamplerList  samplers;
    ConstantList constants;

    textures.Init({
        TextureNode { in_inputSrcTexture, 0, eShaderFlags_PixelShader },
        TextureNode { in_inputDstTexture, 1, eShaderFlags_PixelShader },
    });
    samplers.Init({
        SamplerNode { GetGlobalState()->Sampler_LinearClamp(), 0, eShaderFlags_PixelShader }
    });

    auto cb = CreateConstantBuffer(in_initData);
    constants.Init({ ConstantNode { cb, 0, eShaderFlags_PixelShader } });

    if (out_constantBuffer)
        *out_constantBuffer = cb;

    Ref<ImageFilter> filter = CreateRef<ImageFilter>();
    filter->Init(
        in_width,
        in_height,
        GetGlobalShader()->GetScreenSpaceVS(),
        GetGlobalShader()->GetCombinePS(),
        textures,
        samplers,
        constants);

    return filter;
}

}   // namespace crab