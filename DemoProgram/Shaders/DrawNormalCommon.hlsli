#define MATRIX row_major float4x4

struct VS_Input
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentL : TANGENT;
};

struct GS_Input
{
    float4 posL : SV_POSITION;
    float3 normalL : NORMAL;
};

struct PS_Input
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

cbuffer TransformConstant : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;
};

cbuffer CameraConstant : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePos;
};

cbuffer DrawNormalGSConstant : register(b2)
{
    float g_normalStrength;
}

