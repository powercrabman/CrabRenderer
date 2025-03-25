#include "InternalPCH.h"


#include "SamplingFilter.h"

#include "GlobalShader.h"
#include "RenderStates.h"
#include "ShaderResource.h"

namespace crab
{

SamplingFilter::SamplingFilter()
{
}

SamplingFilter::~SamplingFilter()
{
}

void SamplingFilter::Init(
    uint32                in_width,
    uint32                in_height,
    const Ref<Texture2D>& in_inputTexture)
{
    ImageFilterBase::Init_Internal(
        in_width,
        in_height,
        GlobalShader::GetScreenSpaceVS(),
        GlobalShader::GetSamplingPS(),
        in_inputTexture);
}

}   // namespace crab