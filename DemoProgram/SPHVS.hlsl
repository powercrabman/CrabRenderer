#include "SPHCommon.hlsli"

StructuredBuffer<Particle> g_particles : register(t0);

GS_Input main(uint vertexID : SV_VertexID)
{
    GS_Input output;
    Particle p = g_particles[vertexID];
    output.pos = float4(p.position, 0.f, 1.f);
    output.life = p.life;
    output.radius = p.radius;
    output.color = p.color;
    return output;
}
