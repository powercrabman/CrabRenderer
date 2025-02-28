#include "PostProcessCommon.hlsli"

cbuffer BlurConstants : register(b0)
{
    float dx;
    float dy;
};

Texture2D g_inputTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float x = dx;
    float y = dy;

    // Take 13 samples around the current texel
    float3 a = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-2 * x, 2 * y)).rgb;
    float3 b = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, 2 * y)).rgb;
    float3 c = g_inputTexture.Sample(g_sampler, input.texCoord + float2(2 * x, 2 * y)).rgb;

    float3 d = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-2 * x, 0)).rgb;
    float3 e = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, 0)).rgb;
    float3 f = g_inputTexture.Sample(g_sampler, input.texCoord + float2(2 * x, 0)).rgb;

    float3 g = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-2 * x, -2 * y)).rgb;
    float3 h = g_inputTexture.Sample(g_sampler, input.texCoord + float2(0, -2 * y)).rgb;
    float3 i = g_inputTexture.Sample(g_sampler, input.texCoord + float2(2 * x, -2 * y)).rgb;

    float3 j = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-x, y)).rgb;
    float3 k = g_inputTexture.Sample(g_sampler, input.texCoord + float2(x, y)).rgb;
    float3 l = g_inputTexture.Sample(g_sampler, input.texCoord + float2(-x, -y)).rgb;
    float3 m = g_inputTexture.Sample(g_sampler, input.texCoord + float2(x, -y)).rgb;

    // Apply weighted distribution:
    float3 downsample = e * 0.125;
    downsample += (a + c + g + i) * 0.03125;
    downsample += (b + d + f + h) * 0.0625;
    downsample += (j + k + l + m) * 0.125;

    return float4(downsample, 1.f);
}