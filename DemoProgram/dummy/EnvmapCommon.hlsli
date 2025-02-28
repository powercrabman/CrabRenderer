#define MATRIX row_major float4x4

struct VSInput
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
};

cbuffer CB_Object : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;

    int g_useTexture;
}

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePosW;
};