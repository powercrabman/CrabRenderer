#include "BillboradDemoCommon.hlsli"

static const float g_size = 20.f;

[maxvertexcount(4)] 
void main(
	point GSInput input[1],
	inout TriangleStream<PSInput> output,
	uint primID : SV_PrimitiveID
)
{
	float3 center = mul(input[0].pos, g_world).xyz;

	float3 forward = normalize(g_eyePos - center);
	float3 up = float3(0.f, 1.f, 0.f);
	float3 right = normalize(cross(forward, up));

	float rectHalfSize = g_size * 0.5f;
	
	float3 v1 = center - right * rectHalfSize - up * rectHalfSize;
	float3 v2 = center - right * rectHalfSize + up * rectHalfSize;
	float3 v3 = center + right * rectHalfSize - up * rectHalfSize;
	float3 v4 = center + right * rectHalfSize + up * rectHalfSize;

	PSInput p1, p2, p3, p4;

	p1.pos = mul(float4(v1, 1.f), g_viewProj);
	p1.texCoord = float2(0.f, 1.f);
	p1.primID = primID;

	p2.pos = mul(float4(v2, 1.f), g_viewProj);
	p2.texCoord = float2(0.f, 0.f);
	p2.primID = primID;

	p3.pos = mul(float4(v3, 1.f), g_viewProj);
	p3.texCoord = float2(1.f, 1.f);
	p3.primID = primID;

	p4.pos = mul(float4(v4, 1.f), g_viewProj);
	p4.texCoord = float2(1.f, 0.f);
	p4.primID = primID;

	output.Append(p1);
	output.Append(p2);
	output.Append(p3);
	output.Append(p4);

}
