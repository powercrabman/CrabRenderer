#include "DepthVisualizeCommon.hlsli"

Texture2D g_depthTex : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PS_Input input) : SV_TARGET
{
    float z = g_depthTex.Sample(g_sampler, input.texCoord).r;
    float4 ndc = float4(input.texCoord * 2.0f - 1.0f, z, 1.0f);
    float4 world = mul(ndc, invViewProj);
    world /= world.w;

    float dist = distance(cameraPos, world.xyz);
    float depth = saturate(visualFactor * dist);
    return float4(depth, depth, depth, 1.f);
}
