#define MATRIX row_major float4x4

struct VSInput
{
    float3 position : POSITION;
};

struct HSInput
{
    float4 position : SV_POSITION;
};

struct DSInput
{
    float4 position : SV_POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
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

cbuffer CB_Tessellation : register(b2)
{
    float4 g_edgeTessFactor;
    float2 g_insideTessFactor;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};