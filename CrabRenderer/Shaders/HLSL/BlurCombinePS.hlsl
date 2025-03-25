#include "PostProcessCommon.hlsli"

Texture2D srcTexture : register(t0);
Texture2D dstTexture : register(t1);

float4 PSmain(PS_Input input) : SV_TARGET
{
    float4 srcColor = srcTexture.Sample(SamplerLinearClamp, input.texCoord);
    float4 backBufferColor = dstTexture.Sample(SamplerLinearClamp, input.texCoord);
    float4 finalColor = lerp(backBufferColor, srcColor, cb_combineStrength);
    return finalColor;
}

