#include "PostProcessCommon.hlsli"

Texture2D InputTexture : register(t0);

float4 PSmain(PS_Input input) : SV_TARGET
{
    float4 finalColor = InputTexture.Sample(SamplerLinearClamp, input.texCoord);
    return finalColor;
}