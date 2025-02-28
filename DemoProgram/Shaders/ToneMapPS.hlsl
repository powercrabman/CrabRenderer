#include "PostProcessCommon.hlsli"

cbuffer ToneMapPSConstant : register(b0)
{
    float g_exposure;
    float g_gamma;
}

Texture2D g_inputTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float3 color = g_inputTexture.Sample(g_sampler, input.texCoord).rgb;
    color *= g_exposure;
    
    float3 gammaInv = float3(1.f, 1.f, 1.f) / g_gamma;
    float3 gammaCorrected = pow(max(color, float3(0.f, 0.f, 0.f)), gammaInv);
    
    return float4(gammaCorrected, 1.f);
}