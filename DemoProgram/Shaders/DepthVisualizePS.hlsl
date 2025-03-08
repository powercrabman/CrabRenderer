#include "DepthVisualizeCommon.hlsli"

float4 main(PS_Input input) : SV_TARGET
{
    float z = g_depthMap.Sample(g_sampler, input.texCoord).r;

    float2 ndcXY = input.texCoord * 2.0f - 1.0f;
    float4 ndxPos = float4(ndcXY, z, 1.0f);

    float4 worldPos = mul(ndxPos, g_viewProjInv);
    worldPos.xyz /= worldPos.w;

    float dist = length(worldPos.xyz - g_eyePosW) * g_depthFactor;
    dist = clamp(0.f, 1.f, dist);

    return float4(dist, dist, dist, 1.0f);
}