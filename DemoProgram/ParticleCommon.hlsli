struct Particle
{
    float radius;
    float life;
    float2 position;
    float3 color;

    float2 velocity;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 color : COLOR;
    float life : USERVALUE1;
};


struct GS_Input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float radius : USERVALUE0;
    float life : USERVALUE1;
};