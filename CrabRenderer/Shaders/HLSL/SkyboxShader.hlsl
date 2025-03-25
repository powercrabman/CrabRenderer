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

PS_Input VSmain(VS_3D_INPUT input)
{
    PS_Input output;
    output.posW = mul(float4(input.positionL, 0.f), cb_transformWorld).xyz;
    output.posH = mul(float4(output.posW, 1.f), cb_transformWorldInvTranspose);

    return output;
}

float4 PSmain(PS_Input input) : SV_TARGET
{
    if (g_textureCubeType == 0)
        return g_skyboxCube.Sample(SamplerLinearWrap, input.posW);
    else if (g_textureCubeType == 1)
        return DifCubemap.Sample(SamplerLinearWrap, input.posW);
    else
        return SpecCubemap.Sample(SamplerLinearWrap, input.posW);
}