#define MATRIX row_major float4x4

struct VSInput
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangent : TANGENT;
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
}

cbuffer CB_Camera : register(b1)
{
    MATRIX g_viewProj;
    float3 g_eyePosW;
};

struct Material
{
    float3 albedo;
    float metallic;
    float roughness;
};

struct Light
{
    float3 radiance;
    float fallOffStart;
    float3 position;
    float fallOffEnd;
    float3 direction;
    float spotPower;
};

cbuffer CB_Light : register(b2)
{
    Light light;
};

cbuffer CB_ObjectPixelShader : register(b3)
{
    Material g_material;
    float3 pad;

    int g_useHeightMap;
    float g_heightScale;
    int g_useNormalMap;
    int g_useAlbedoMap;

    int g_useRoughnessMap;
    int g_useMetallicMap;
    int g_useAOMap;
};