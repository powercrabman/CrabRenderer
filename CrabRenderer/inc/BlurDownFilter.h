#pragma once
#include "ImageFilterBase.h"

namespace crab
{

class Texture2D;

class BlurDownFilter : public ImageFilterBase
{
public:
    BlurDownFilter();
    ~BlurDownFilter() override;

    void Init(uint32                in_width,
              uint32                in_height,
              const Ref<Texture2D>& in_inputTexture);

    void Bind() override;

    void SetBlurDownFactor(float in_dx, float in_dy) const;

private:
    Ref<ConstantBuffer<CB_BLUR_DOWN>> m_blurDownConstant;
};

}   // namespace crab
