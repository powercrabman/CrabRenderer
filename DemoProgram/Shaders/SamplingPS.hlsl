#include "PostProcessCommon.hlsli"

Texture2D g_inputTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 finalColor = g_inputTexture.Sample(g_sampler, input.texCoord);
    return finalColor;
}