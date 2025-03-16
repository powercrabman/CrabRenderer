#include "ShaderCommon.hlsli"

struct PS_Input
{
    float3 posW : POSITION;
    float4 posH : SV_POSITION;
};

cbuffer SkyboxPSConstant : register(b2)
{
    int g_textureCubeType; // 0 or 1 or 2
};

TextureCube g_skyboxCube : register(t10);

PS_Input VSmain(VS_Input input)
{
    PS_Input output;
    output.posW = mul(float4(input.posL, 1.f), g_world).xyz;
    output.posH = mul(float4(output.posW, 1.f), g_viewProj);

    return output;
}

float4 PSmain(PS_Input input) : SV_TARGET
{
    if (g_textureCubeType == 1)
    {
        return g_diffuseCube.Sample(g_wrapSampler, input.posW);
    }
    else if (g_textureCubeType == 2)
    {
        return g_specularCube.Sample(g_wrapSampler, input.posW);
    }

    return g_skyboxCube.Sample(g_wrapSampler, input.posW);
}