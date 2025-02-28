#include "PostProcessCommon.hlsli"

cbuffer CB_BlurCombine : register(b0)
{
    float g_strength;
};

Texture2D g_srcTexture : register(t0);
Texture2D g_backBuffer : register(t1);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 srcColor = g_srcTexture.Sample(g_sampler, input.texCoord);
    float4 backBufferColor = g_backBuffer.Sample(g_sampler, input.texCoord);
    float4 finalColor = lerp(backBufferColor, srcColor, g_strength);
    return finalColor;
}

