#include "ShaderCommon.hlsli"

TextureCube g_skyboxCube : register(t10);

cbuffer SkyboxPSConstant : register(b2)
{
    int g_textureCubeType; // 0 or 1 or 2
};

float4 main(PS_Input input) : SV_TARGET
{
    if (g_textureCubeType == 0)
    {
        return g_skyboxCube.Sample(g_wrapSampler, input.posW);
    }
    else if (g_textureCubeType == 1)
    {
        return g_irradiateCube.Sample(g_wrapSampler, input.posW);
    }
    else if (g_textureCubeType == 2)
    {
        return g_specularCube.Sample(g_wrapSampler, input.posW);
    }

    return float4(0.f, 0.f, 0.f, 1.f);
}