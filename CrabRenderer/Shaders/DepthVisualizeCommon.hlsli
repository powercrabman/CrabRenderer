#define MATRIX row_major float4x4

cbuffer DepthVisualizeConstant : register(b0)
{
    MATRIX g_viewProjInv;
    // -------------------------------------
    float3 g_eyePosW;
    float g_depthFactor;
};

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

Texture2D g_depthMap : register(t0);
SamplerState g_sampler : register(s0);