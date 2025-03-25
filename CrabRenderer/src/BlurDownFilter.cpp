#include "InternalPCH.h"

#include "BlurDownFilter.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"

namespace crab
{

BlurDownFilter::BlurDownFilter()
{
}

BlurDownFilter::~BlurDownFilter()
{
}

void BlurDownFilter::Init(uint32                in_width,
                          uint32                in_height,
                          const Ref<Texture2D>& in_inputTexture)
{
    ImageFilterBase::Init_Internal(
        in_width,
        in_height,
        GlobalShader::GetScreenSpaceVS(),
        GlobalShader::GetBlurDownPS(),
        in_inputTexture);

    m_blurDownConstant = GlobalConstants::GetBlurDownConstant();
}

void BlurDownFilter::Bind()
{
    ImageFilterBase::Bind();

    m_blurDownConstant->Bind(0, eShaderFlags_PixelShader);
}

void BlurDownFilter::SetBlurDownFactor(float in_dx, float in_dy) const
{
    m_blurDownConstant->WriteToBuffer(CB_BLUR_DOWN {
        .cb_blurDownDeltaX = in_dx,
        .cb_blurDownDeltaY = in_dy });
}

}   // namespace crab