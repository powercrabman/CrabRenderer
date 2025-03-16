#include "PBRCommon.hlsli"

Texture2D g_heightMap : register(t0);
SamplerState g_linearSampler : register(s0);

PSInput main(VSInput input)
{
    PSInput output;

    float3 posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    float3 normalW = normalize(mul(input.normalL, (float3x3) g_worldInvTranspose));

    if (g_useHeightMap)
    {
        float heightSample = g_heightMap.SampleLevel(g_linearSampler, input.texCoord, 0).r;
        heightSample = heightSample * 2.f - 1.f;

        posW += normalW * heightSample * g_heightScale;
    }

    float3 tangentW = normalize(mul(input.tangent, (float3x3) g_world));

    output.posH = mul(float4(posW, 1.f), g_viewProj);
    output.posW = posW;
    output.normalW = normalW;
    output.texCoord = input.texCoord;
    output.tangentW = tangentW;
    return output;
}