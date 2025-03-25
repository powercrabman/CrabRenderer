#pragma once
#include "ImageFilterBase.h"

namespace crab
{

class ToneMapFilter : public ImageFilterBase
{
public:
    ToneMapFilter();
    ~ToneMapFilter() override;

    void Init(uint32                in_width,
              uint32                in_height,
              const Ref<Texture2D>& in_inputTexture);

    void Bind() override;

    void SetToneMappingFactor(float in_exposure,
                              float in_gamma) const;

private:
    Ref<ConstantBuffer<CB_TONE_MAPPING>> m_toneMappingConstant;
};

}   // namespace crab