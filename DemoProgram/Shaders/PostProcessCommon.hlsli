struct VS_Input
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};