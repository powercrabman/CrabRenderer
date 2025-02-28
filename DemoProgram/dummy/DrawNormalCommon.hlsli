#define MATRIX row_major float4x4

struct VSInput
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct GSInput
{
    float4 posL : SV_POSITION;
    float3 normalL : NORMAL;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
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
};

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePos;
};

cbuffer CB_PixelShader : register(b2)
{
    float g_dx;
    float g_dy;
    float g_deltaTime;
}