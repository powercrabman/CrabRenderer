#include "ShaderCommon.hlsli"

struct PS_Input
{
    float4 posH : SV_POSITION;
};

PS_Input VSmain(VS_Input input)
{
    PS_Input output;
    float4 powW = mul(float4(input.posL, 1.0f), g_world);
    output.posH = mul(powW, g_viewProj);

    return output;
}

float4 PSmain(PS_Input input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}