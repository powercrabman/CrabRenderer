#include "PostProcessCommon.hlsli"

cbuffer BlurUpConstant : register(b0)
{
    float radius;
};

Texture2D g_inputTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float x = radius;
    float y = radius;

    // Take 9 samples around current texel
    float3 a = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-x, y)).rgb;
    float3 b = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, y)).rgb;
    float3 c = g_inputTexture.Sample(g_sampler, input.texCoord + float2(x, y)).rgb;

    float3 d = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-x, 0)).rgb;
    float3 e = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, 0)).rgb;
    float3 f = g_inputTexture.Sample(g_sampler, input.texCoord + float2(x, 0)).rgb;

    float3 g = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-x, -y)).rgb;
    float3 h = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, -y)).rgb;
    float3 i = g_inputTexture.Sample(g_sampler, input.texCoord + float2(x, -y)).rgb;

    // Apply weighted distribution using a 3x3 tent filter
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    float3 upsample = e * 4.0;
    upsample += (b + d + f + h) * 2.0;
    upsample += (a + c + g + i);
    upsample *= 1.0 / 16.0;

    return float4(upsample, 1.f);
}