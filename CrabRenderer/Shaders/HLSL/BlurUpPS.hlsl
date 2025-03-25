#include "PostProcessCommon.hlsli"

Texture2D inputTexture : register(t0);

float4 PSmain(PS_Input input) : SV_TARGET
{
    float x = cb_blurUpRadius;
    float y = cb_blurUpRadius;

    // Take 9 samples around current texel
    float3 a = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-x, y)).rgb;
    float3 b = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, y)).rgb;
    float3 c = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(x, y)).rgb;

    float3 d = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-x, 0)).rgb;
    float3 e = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, 0)).rgb;
    float3 f = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(x, 0)).rgb;

    float3 g = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(-x, -y)).rgb;
    float3 h = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(0, -y)).rgb;
    float3 i = inputTexture.Sample(SamplerLinearClamp, input.texCoord + float2(x, -y)).rgb;

    // Apply weighted distribution using a 3x3 tent filter
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    float3 upSample = e * 4.0;
    upSample += (b + d + f + h) * 2.0;
    upSample += (a + c + g + i);
    upSample *= 1.0 / 16.0;

    return float4(upSample, 1.f);
}