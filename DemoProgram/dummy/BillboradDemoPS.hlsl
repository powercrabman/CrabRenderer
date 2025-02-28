#include "BillboradDemoCommon.hlsli"

Texture2DArray g_textureArray : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 color = g_textureArray.Sample(g_sampler, float3(input.texCoord, input.primID % 5));

    float colorAvg = (color.r + color.g + color.b) / 3.f;
    clip(0.75f - colorAvg);
    return color;
}
