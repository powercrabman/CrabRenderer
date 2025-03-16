struct PSInput
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
};

TextureCube g_cubeMap : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	return g_cubeMap.Sample(g_sampler, input.posW.xyz);
}