#include "PostProcessCommon.hlsli"

Texture2D InputTexture : register(t0);

float4 PSmain(PS_Input input) : SV_TARGET
{
    float3 color = InputTexture.Sample(SamplerLinearClamp, input.texCoord).rgb;
    color *= cb_toneMappingExposure;
    
    float3 gammaInv = float3(1.f, 1.f, 1.f) / cb_toneMappingGamma;
    float3 gammaCorrected = pow(max(color, float3(0.f, 0.f, 0.f)), gammaInv);
    
    return float4(gammaCorrected, 1.f);
}