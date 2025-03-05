#define MATRIX row_major float4x4

struct VS_Input
{
    float2 posL : POSITION;
    float2 texCoord : TEXCOORD;
    
};

struct PS_Input
{
    float4 posH : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer DepthVisualizeConstant : register(b0)
{
    MATRIX invViewProj; // 16
    // -----------------------------------
    float3 cameraPos; // 12
    float visualFactor; // 4
    // -----------------------------------
};