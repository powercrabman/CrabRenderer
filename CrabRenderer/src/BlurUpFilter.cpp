#include "InternalPCH.h"

#include "BlurUpFilter.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"

namespace crab
{
BlurUpFilter::BlurUpFilter()
{
}

BlurUpFilter::~BlurUpFilter()
{
}

void BlurUpFilter::Init(uint32                in_width,
                        uint32                in_height,
                        const Ref<Texture2D>& in_inputTexture)
{
    ImageFilterBase::Init_Internal(
        in_width,
        in_height,
        GlobalShader::GetScreenSpaceVS(),
        GlobalShader::GetBlurUpPS(),
        in_inputTexture);

    m_blurUpConstant = GlobalConstants::GetBlurUpConstant();
}

void BlurUpFilter::Bind()
{
    ImageFilterBase::Bind();

    m_blurUpConstant->Bind(0, eShaderFlags_PixelShader);
}

void BlurUpFilter::SetBlurRadius(float in_blurRadius) const
{
    m_blurUpConstant->WriteToBuffer(CB_BLUR_UP { in_blurRadius });
}

}   // namespace crab