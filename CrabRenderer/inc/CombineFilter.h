#pragma once
#include "ImageFilterBase.h"

namespace crab
{
struct CB_COMBINE;
class Texture2D;

class CombineFilter : public ImageFilterBase
{
public:
    CombineFilter();
    ~CombineFilter() override;

    void Init(uint32                in_width,
              uint32                in_height,
              const Ref<Texture2D>& in_inputSrcTexture,
              const Ref<Texture2D>& in_inputDstTexture);

    void Bind() override;

    void SetCombineStrength(float in_combineStrength) const;

private:
    Ref<ConstantBuffer<CB_COMBINE>> m_combineConstant;
    Ref<Texture2D>                  m_inputDstTexture;
};

}   // namespace crab
