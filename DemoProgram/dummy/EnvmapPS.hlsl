#include "EnvmapCommon.hlsli"

TextureCube g_envMap : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float3 envColor = g_envMap.Sample(g_sampler, input.posW).rgb;
    return float4(envColor, 1.0f);
}