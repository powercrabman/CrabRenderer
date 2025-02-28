#define Mat4 row_major float4x4

cbuffer CB_Object : register(b0)
{
    Mat4 g_world;
    Mat4 g_worldInvTranspose;

    float4 g_ambient;
    float4 g_diffuse;
    float4 g_specular;
    float g_shininess;

    bool  g_useTexture;
};

cbuffer CB_Camera : register(b1)
{
    Mat4 g_viewProj;
    float3 g_eyePos;
};

cbuffer CB_PixelShader : register(b2)
{
    float g_diffuseMapStrength;
    float g_specularMapStrength;
    float g_speculerShininess;
    bool  g_useSmoothStep;
};

struct VSInput
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};