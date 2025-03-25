#include "PostProcessCommon.hlsli"

Texture2D inputTexture : register(t0);

float4 PSmain(PS_Input input) : SV_TARGET
{
    float x = cb_blurDownDeltaX;
    float y = cb_blurDownDeltaY;

    // Take 13 samples around the current texel
    float3 a = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-2 * x, 2 * y)).rgb;
    float3 b = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, 2 * y)).rgb;
    float3 c = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(2 * x, 2 * y)).rgb;

    float3 d = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-2 * x, 0)).rgb;
    float3 e = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, 0)).rgb;
    float3 f = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(2 * x, 0)).rgb;

    float3 g = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-2 * x, -2 * y)).rgb;
    float3 h = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, -2 * y)).rgb;
    float3 i = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(2 * x, -2 * y)).rgb;

    float3 j = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-x, y)).rgb;
    float3 k = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(x, y)).rgb;
    float3 l = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-x, -y)).rgb;
    float3 m = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(x, -y)).rgb;

    // Apply weighted distribution:
    float3 downSample = e * 0.125;
    downSample += (a + c + g + i) * 0.03125;
    downSample += (b + d + f + h) * 0.0625;
    downSample += (j + k + l + m) * 0.125;

    return float4(downSample, 1.f);
}