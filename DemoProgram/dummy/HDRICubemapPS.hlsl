#include "HDRICubemapCommon.hlsli"

TextureCube g_envMap : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    return g_envMap.Sample(g_sampler, input.posW);
}