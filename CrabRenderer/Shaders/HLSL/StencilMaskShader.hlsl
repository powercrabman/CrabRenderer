#include "ShaderCommon.hlsli"

struct PS_Input
{
    float4 posH : SV_POSITION;
};

PS_Input VSmain(VS_3D_INPUT input)
{
    PS_Input output;
    float4 powW = mul(float4(input.positionL, 1.0f), cb_transformWorld);
    output.posH = mul(powW, cb_cameraViewProj);

    return output;
}

float PSmain(PS_Input input) : SV_Depth
{
    return input.posH.z;
}