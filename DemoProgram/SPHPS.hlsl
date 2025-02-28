#include "SPHCommon.hlsli"

float SmootherStep(float edge0, float edge1, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * x * (x * (6.0f * x - 15.0f) + 10.0f);
}

float4 main(PS_Input input) : SV_TARGET
{
    float dist = distance(input.texCoord, float2(0.5f, 0.5f));
    float lifeFactor = clamp(input.life, 0.f, 1.f);
    float factor = (0.5f - dist) * 2.f * lifeFactor;
    factor = SmootherStep(0.f, 1.f, factor);
    float3 finalColor = input.color;
    return float4(finalColor, factor);
}