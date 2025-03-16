#pragma once
#include "Buffers.h"
#include "Constants.h"

namespace crab
{

class ImageFilter;
class Texture2D;

class ImageFilterFactory
{
public:
    static Ref<ImageFilter> CreateBlurDown(
        uint32                                 in_width,
        uint32                                 in_height,
        const Ref<Texture2D>&                  in_inputTexture,
        Ref<ConstantBuffer<BlurDownConstant>>* out_constantBuffer = nullptr);

    static Ref<ImageFilter> CreateBlurUp(
        uint32                               in_width,
        uint32                               in_height,
        const Ref<Texture2D>&                in_inputTexture,
        const BlurUpConstant&                in_initData        = {},
        Ref<ConstantBuffer<BlurUpConstant>>* out_constantBuffer = nullptr);

    static Ref<ImageFilter> CreateSampling(
        uint32                in_width,
        uint32                in_height,
        const Ref<Texture2D>& in_inputTexture);

    static Ref<ImageFilter> CreateToneMapping(
        uint32                                    in_width,
        uint32                                    in_height,
        const Ref<Texture2D>&                     in_inputTexture,
        const ToneMappingConstant&                in_initData        = {},
        Ref<ConstantBuffer<ToneMappingConstant>>* out_constantBuffer = nullptr);

    static Ref<ImageFilter> CreateCombine(
        uint32                                in_width,
        uint32                                in_height,
        const Ref<Texture2D>&                 in_inputSrcTexture,
        const Ref<Texture2D>&                 in_inputDstTexture,
        const CombineConstant&                in_initData        = {},
        Ref<ConstantBuffer<CombineConstant>>* out_constantBuffer = nullptr);
};

}   // namespace crab