#include "PostProcessingCommon.hlsli"


float4 main(PSInput input) : SV_TARGET
{
    if (input.texCoord.x < g_threshold)
    {
        return g_texture.Sample(g_sampler, input.texCoord);
    }
    else
    {
        return float4(0.0f, 0.0f, 1.0f, 1.0f);
    }
}