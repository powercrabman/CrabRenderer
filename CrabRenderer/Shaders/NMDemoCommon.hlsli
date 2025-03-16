#define MATRIX row_major float4x4

struct VSInput
{
    float3 posL : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normalL : NORMAL;
    float3 tangentL : TANGENT;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentW : TANGENT;
};

cbuffer CB_Object : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;

    bool g_useTexture;
};

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePosW;
}

cbuffer CB_Light : register(b2)
{
    float3 g_lightPosW;
    bool pad;
    
    bool g_useDiffuseTexture;
    bool g_useNormalTexture;
};