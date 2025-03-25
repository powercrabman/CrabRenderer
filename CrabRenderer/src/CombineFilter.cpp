#include "InternalPCH.h"

#include "CombineFilter.h"

#include "GlobalConstants.h"
#include "GlobalShader.h"
#include "ImageFilterBase.h"
#include "ShaderResource.h"

namespace crab
{

CombineFilter::CombineFilter()
{
}

CombineFilter::~CombineFilter()
{
}

void CombineFilter::Init(uint32                in_width,
                         uint32                in_height,
                         const Ref<Texture2D>& in_inputSrcTexture,
                         const Ref<Texture2D>& in_inputDstTexture)
{
    ImageFilterBase::Init_Internal(
        in_width,
        in_height,
        GlobalShader::GetScreenSpaceVS(),
        GlobalShader::GetCombinePS(),
        in_inputSrcTexture);

    m_inputDstTexture = in_inputDstTexture;
    m_combineConstant = GlobalConstants::GetCombineConstant();
}

void CombineFilter::Bind()
{
    ImageFilterBase::Bind();

    m_inputDstTexture->Bind(1, eShaderFlags_PixelShader);
    m_combineConstant->Bind(0, eShaderFlags_PixelShader);
}

void CombineFilter::SetCombineStrength(float in_combineStrength) const
{
    m_combineConstant->WriteToBuffer(CB_COMBINE { .cb_combineStrength = in_combineStrength });
}

}   // namespace crab
