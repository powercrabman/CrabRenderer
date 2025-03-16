#include "LightDemoCommon.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 PSmain(PSInput input) : SV_TARGET
{
    return float4(g_lightColor, 1.f);
}
