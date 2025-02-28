struct VSInput
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer CB_PostProcessing : register(b0)
{
    float g_dx;
    float g_dy;
    float g_threshold;
    float g_strength;
    float4 g_userValues;
};
