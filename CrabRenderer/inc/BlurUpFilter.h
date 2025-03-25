#pragma once
#include "ImageFilterBase.h"

namespace crab
{

class BlurUpFilter : public ImageFilterBase
{
public:
    BlurUpFilter();
    ~BlurUpFilter() override;

    void Init(uint32                in_width,
              uint32                in_height,
              const Ref<Texture2D>& in_inputTexture);

    void Bind() override;

    void SetBlurRadius(float in_blurRadius) const;

private:
    Ref<ConstantBuffer<CB_BLUR_UP>> m_blurUpConstant;
};

}   // namespace crab