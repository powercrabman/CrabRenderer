#include "PostProcessCommon.hlsli"

cbuffer CB_BlurSample : register(b0)
{
    float dx;
    float dy;
};

Texture2D g_srcTexture : register(t0);
SamplerState g_sampler : register(s0);

static const int RADIUS = 4;
static const float SIGMA = 8.f;

float Gaussian(float x, float sigma)
{
    return exp(-0.5 * (x * x) / (sigma * sigma));
}

float4 main(PSInput input) : SV_TARGET
{
    float3 color = float3(0, 0, 0);
    float weightSum = 0.0;

    for (int i = -RADIUS; i <= RADIUS; i++)
    {
        float weight = Gaussian(float(i), SIGMA);
        float2 offset = float2(0, dy * i);
        color += g_srcTexture.Sample(g_sampler, input.texCoord + offset).rgb * weight;
        weightSum += weight;
    }

    return float4(color / weightSum, 1.0);
}