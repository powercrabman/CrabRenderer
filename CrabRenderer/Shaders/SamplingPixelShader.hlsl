#include "PostProcessingCommon.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.texCoord);
}