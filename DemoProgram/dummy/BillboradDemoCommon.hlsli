#define MATRIX row_major float4x4

struct VSInput
{
	float3 pos : POSITION;
};

struct GSInput
{
	float4 pos : SV_POSITION;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	uint primID : SV_PrimitiveID;
};

cbuffer CB_Object : register(b0)
{
	MATRIX g_world;
	MATRIX g_worldInvTranspose;
	float4 g_ambient;
	float4 g_diffuse;
	float4 g_specular;

	float g_shininess;
	bool g_useTexture;
	uint g_entityID;
};

cbuffer CB_Camera : register(b1)
{
	MATRIX g_viewProj;
	float3 g_eyePos;
};