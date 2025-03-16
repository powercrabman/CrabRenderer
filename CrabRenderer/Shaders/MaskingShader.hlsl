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

float PSmain(PS_Input input) : SV_Depth
{
    return input.posH.z;
}