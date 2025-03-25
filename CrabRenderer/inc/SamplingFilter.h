#pragma once
#include "ImageFilterBase.h"

namespace crab
{

class SamplingFilter : public ImageFilterBase
{
public:
    SamplingFilter();
    ~SamplingFilter() override;

    void Init(uint32                in_width,
              uint32                in_height,
              const Ref<Texture2D>& in_inputTexture);
};

}   // namespace crab
