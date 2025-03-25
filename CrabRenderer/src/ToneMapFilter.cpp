#include "InternalPCH.h"


#include "ToneMapFilter.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"
#include "GlobalState.h"
#include "ImageFilterBase.h"
#include "RenderFactory.h"

namespace crab
{

ToneMapFilter::ToneMapFilter()
{
}

ToneMapFilter::~ToneMapFilter()
{
}

void ToneMapFilter::Init(uint32                in_width,
                         uint32                in_height,
                         const Ref<Texture2D>& in_inputTexture)
{
    ImageFilterBase::Init_Internal(
        in_width,
        in_height,
        GlobalShader::GetScreenSpaceVS(),
        GlobalShader::GetToneMapPS(),
        in_inputTexture);

    m_toneMappingConstant = GlobalConstants::GetToneMappingConstant();
}

void ToneMapFilter::Bind()
{
    ImageFilterBase::Bind();

    m_toneMappingConstant->Bind(0, eShaderFlags_PixelShader);
}

void ToneMapFilter::SetToneMappingFactor(float in_exposure, float in_gamma) const
{
    m_toneMappingConstant->WriteToBuffer(
        CB_TONE_MAPPING {
            in_exposure,
            in_gamma });
}

}   // namespace crab