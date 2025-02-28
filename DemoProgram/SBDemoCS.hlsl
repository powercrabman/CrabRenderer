struct Particle
{
    float2 position;
    float4 color;
};

RWStructuredBuffer<Particle> g_particles : register(u0);

#define DELTA_TIME 1.0f / 240.0f
#define DELTA_ANGLE 0.01f

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 pos = g_particles[DTid.x].position;
    float2 normalV = normalize(float2(-pos.y, pos.x));
    float2 nextPos = pos + normalV * DELTA_ANGLE;
    g_particles[DTid.x].position = nextPos;
}