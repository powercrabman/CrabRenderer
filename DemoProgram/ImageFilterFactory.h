#pragma once
#include "Constants.h"
#include "D11ResourceList.h"

class ImageFilterFactory
{
public:
    static Ref<ImageFilter> CreateBlurDown(
        uint32                                 in_filterWidth,
        uint32                                 in_filterHeight,
        const Ref<SamplerState>&               in_sampler,
        const Ref<Image2D>&                    in_inputImage,
        const BlurDownConstant&                in_initData,
        Ref<ConstantBuffer<BlurDownConstant>>* out_constants)
    {
        SamplerList samplers;
        samplers.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputImage, 0, eShaderFlags_PixelShader);

        auto         constant = ConstantBuffer<BlurDownConstant>::Create(in_initData);
        ConstantList constants;
        constants.Add(constant, 0, eShaderFlags_PixelShader);

        if (out_constants)
            *out_constants = constant;

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().BlurDownPixelShader(),
            images,
            samplers,
            constants);
    }

    static Ref<ImageFilter> CreateBlurUp(
        uint32                               in_filterWidth,
        uint32                               in_filterHeight,
        const Ref<SamplerState>&             in_sampler,
        const Ref<Image2D>&                  in_inputImage,
        const BlurUpConstant&                in_initData,
        Ref<ConstantBuffer<BlurUpConstant>>* out_constants)
    {
        SamplerList sampler;
        sampler.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputImage, 0, eShaderFlags_PixelShader);

        auto         constant = ConstantBuffer<BlurUpConstant>::Create(in_initData);
        ConstantList constants;
        constants.Add(constant, 0, eShaderFlags_PixelShader);

        if (out_constants)
            *out_constants = constant;

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().BlurUpPixelShader(),
            images,
            sampler,
            constants);
    }

    static Ref<ImageFilter> CreateSampling(
        uint32                   in_filterWidth,
        uint32                   in_filterHeight,
        const Ref<SamplerState>& in_sampler,
        const Ref<Image2D>&      in_inputImage)
    {
        SamplerList sampler;
        sampler.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputImage, 0, eShaderFlags_PixelShader);

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().SamplingPixelShader(),
            images,
            sampler,
            ConstantList());
    }

    static Ref<ImageFilter> CreateToneMapping(
        uint32                                  in_filterWidth,
        uint32                                  in_filterHeight,
        const Ref<SamplerState>&                in_sampler,
        const Ref<Image2D>&                     in_inputImage,
        const ToneMapConstant&                in_initData,
        Ref<ConstantBuffer<ToneMapConstant>>* out_constants)
    {
        SamplerList sampler;
        sampler.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputImage, 0, eShaderFlags_PixelShader);

        auto         constant = ConstantBuffer<ToneMapConstant>::Create(in_initData);
        ConstantList constants;
        constants.Add(constant, 0, eShaderFlags_PixelShader);

        if (out_constants)
            *out_constants = constant;

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().ToneMappingPixelShader(),
            images,
            sampler,
            constants);
    }

    static Ref<ImageFilter> CreateCombine(
        uint32                                  in_filterWidth,
        uint32                                  in_filterHeight,
        const Ref<SamplerState>&                in_sampler,
        const Ref<Image2D>&                     in_inputSrcImage,
        const Ref<Image2D>&                     in_inputDestImage,
        const CombineConstant&                in_initData,
        Ref<ConstantBuffer<CombineConstant>>* out_constants)
    {
        SamplerList sampler;
        sampler.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputSrcImage, 0, eShaderFlags_PixelShader);
        images.Add(in_inputDestImage, 1, eShaderFlags_PixelShader);

        auto         constant = ConstantBuffer<CombineConstant>::Create(in_initData);
        ConstantList constants;
        constants.Add(constant, 0, eShaderFlags_PixelShader);

        if (out_constants)
            *out_constants = constant;

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().CombinePixelShader(),
            images,
            sampler,
            constants);
    }

    static Ref<ImageFilter> CreatePostEffect(
        uint32                                   in_filterWidth,
        uint32                                   in_filterHeight,
        const Ref<SamplerState>&                 in_sampler,
        const Ref<Image2D>&                      in_inputImage,
        const PostEffectConstant&                in_initData,
        Ref<ConstantBuffer<PostEffectConstant>>* out_constants)
    {
        SamplerList sampler;
        sampler.Add(in_sampler, 0, eShaderFlags_PixelShader);

        Image2DList images;
        images.Add(in_inputImage, 0, eShaderFlags_PixelShader);

        auto         constant = ConstantBuffer<PostEffectConstant>::Create(in_initData);
        ConstantList constants;
        constants.Add(constant, 0, eShaderFlags_PixelShader);

        if (out_constants)
            *out_constants = constant;

        return ImageFilter::Create(
            in_filterWidth,
            in_filterHeight,
            GetShaderLibrary().PostProcessVertexShader(),
            GetShaderLibrary().PostEffectPixelShader(),
            images,
            sampler,
            constants);
    }
};
