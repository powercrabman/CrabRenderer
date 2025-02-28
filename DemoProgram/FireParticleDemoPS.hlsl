#include "FireParticleDemoCommon.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float SmootherStep(float edge0, float edge1, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * x * (x * (6.0f * x - 15.0f) + 10.0f);
}

float4 main(PS_Input input) : SV_TARGET
{
    float lifeFactor = clamp(input.life, 0.f, 1.f);
    float factor = lifeFactor;
    factor = SmootherStep(0.f, 1.f, factor);

    input.texCoord.x = lerp(input.texCoord.x * -1 + 1, input.texCoord.x, input.primID % 2);

    float3 texColor =
        g_texture.Sample(g_sampler, input.texCoord).rgb;

    return float4(texColor, factor);
}