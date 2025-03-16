struct Particle
{
    float2 position;
    float4 color;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

StructuredBuffer<Particle> g_Particles : register(t0);

PS_Input main(uint vertexID : SV_VertexID)
{
    PS_Input output;
    Particle p = g_Particles[vertexID];
    output.pos = float4(p.position, 0.f, 1.f);
    output.color = p.color;
    return output;
}