#include "DrawNormalCommon.hlsli"

static const float g_normalLineLength = 0.5f;

[maxvertexcount(6)]
void main(
	triangle GSInput input[3] : SV_POSITION,
	inout LineStream<PSInput> output
)
{
	for (int i = 0; i < 3; i++)
	{
		float3 p1 = input[i].posL.xyz;
		float3 p2 = input[i].posL.xyz + g_normalLineLength * input[i].normalL;
		
		float4 p1World = mul(float4(p1, 1.f), g_world);
		float4 p1H = mul(p1World, g_viewProj);

		float4 p2World = mul(float4(p2, 1.f), g_world);
		float4 p2H = mul(p2World, g_viewProj);
		
		PSInput data1;
		data1.posH = p1H;
		data1.color = float4(1.f, 0.f, 0.f, 1.f);
		
		PSInput data2;
		data2.posH = p2H;
		data2.color = float4(1.f, 1.f, 0.f, 1.f);
		
		output.Append(data1);
		output.Append(data2);
		output.RestartStrip();
	}
}