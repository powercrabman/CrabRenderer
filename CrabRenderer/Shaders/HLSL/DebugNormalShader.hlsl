#include "ShaderCommon.hlsli"

struct GS_Input
{
    float4 posW : SV_POSITION;
    float3 normalW : NORMAL;
};

struct PS_Input
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

GS_Input VSmain(VS_3D_INPUT input)
{
    GS_Input output;
    output.posW = mul(float4(input.positionL, 1.f), cb_transformWorld);
    output.normalW = mul(float4(input.normalL, 0.f), cb_transformWorldInvTranspose).xyz;

    return output;
}

[maxvertexcount(6)]
void GSmain(
	triangle GS_Input input[3],
	inout LineStream<PS_Input> output
)
{
    for (int i = 0; i < 3; i++)
    {
        PS_Input output1;
        PS_Input output2;

        output1.posH = mul(input[i].posW, cb_cameraViewProj);
        output1.color = float4(1.f, 0.f, 0.f, 1.f);

        output2.posH = mul(input[i].posW + float4(input[i].normalW, 0.f) * 0.1f, cb_cameraViewProj);
        output2.color = float4(1.f, 1.f, 0.f, 1.f);
		
        output.Append(output1);
        output.Append(output2);
        output.RestartStrip();
    }

}

float4 PSmain(PS_Input input) : SV_TARGET
{
    return input.color;
}