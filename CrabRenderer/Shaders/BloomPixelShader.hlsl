#include "PostProcessingCommon.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    float3 color = g_texture.Sample(g_sampler, input.texCoord).rgb;
    float meanValue = (color.r + color.g + color.b) / 3.0f;
    
    if (meanValue < g_threshold)
    {
        return float4(0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        return float4(color, 1.f);
    }
}