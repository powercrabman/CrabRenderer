#include "NMDemoCommon.hlsli"

Texture2D g_heightTexture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer CB_HeightMap : register(b2)
{
    bool useHeightMap;
    float heightScale;
};

PSInput main(VSInput input)
{
    PSInput output;
    
    output.posW = mul(float4(input.normalL, 1.0f), g_world).xyz;
    output.normalW = mul(float4(input.normalL, 1.0f), g_worldInvTranspose).xyz;
    output.normalW = normalize(output.normalW);
    output.texCoord = input.texCoord;
    output.tangentW = mul(float4(input.tangentL, 1.0f), g_world).xyz;
    
    if (useHeightMap)
    {
        float height = g_heightTexture.SampleLevel(g_sampler, input.texCoord, 0).r;
        height = (height - 0.5f) * 2.0f;
        output.posW += height * output.normalW * heightScale;
    }
    
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);

    return output;
}