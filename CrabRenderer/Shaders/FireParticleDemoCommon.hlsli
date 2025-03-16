struct Particle
{
    float2 position;
    float2 velocity;
    float life;
    float scale;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    uint primID : PRIMITIVEID;
    float life : USERVALUE1;
};

struct GS_Input
{
    float4 pos : SV_POSITION;
    float life : USERVALUE1;
    float scale : USERVALUE2;
};