#include "DrawNormalCommon.hlsli"

[maxvertexcount(6)]
void main(
	triangle GS_Input input[3], 
	inout LineStream<PS_Input> output
)
{
    for (int i = 0; i < 3; i++)
    {
        float3 p1 = input[i].posL.xyz;
        float3 p2 = input[i].posL.xyz + g_normalStrength * input[i].normalL;
		
        float3 p1World = mul(float4(p1, 1.f), g_world).xyz;
        float4 p1H = mul(float4(p1World, 1.f), g_viewProj);

        float3 p2World = mul(float4(p2, 1.f), g_world).xyz;
        float4 p2H = mul(float4(p2World, 1.f), g_viewProj);
		
        PS_Input data1;
        data1.posH = p1H;
        data1.color = float4(1.f, 0.f, 0.f, 1.f);
		
        PS_Input data2;
        data2.posH = p2H;
        data2.color = float4(1.f, 1.f, 0.f, 1.f);
		
        output.Append(data1);
        output.Append(data2);
        output.RestartStrip();
    }

}