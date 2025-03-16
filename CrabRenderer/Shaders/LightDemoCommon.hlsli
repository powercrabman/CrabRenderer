#define MATRIX row_major float4x4

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

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    float3 pad;
};

cbuffer CB_Object : register(b0)
{
    MATRIX   g_world; //64
    MATRIX   g_worldInvTranspose; //64
    Material g_material; // 64
    bool     g_useTexture; // 4
};

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePosW;
};

cbuffer CB_Light : register(b2)
{
    float3 g_lightColor;
    float  g_lightIntensity;

    float3 g_lightPosW;
    float  g_falloffStart;

    float  g_falloffEnd;
    float3 pad;
    
    bool  g_useSmoothStep;
};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}