#include "FireParticleDemoCommon.hlsli"

#define GRAVITY_CONSTANT (0.098f * 2.f)
#define SMALL_NUMBER (0.0001f)
#define INV_ASPECT_RATIO (1080.f/1920.f)

cbuffer ParticleConstant : register(b0)
{
    float g_deltaTime;
    float3 pad;
};

RWStructuredBuffer<Particle> g_output;

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint i = DTid.y * 32 + DTid.x;
    Particle p = g_output[i];

    p.life += -2.f * g_deltaTime;

    p.velocity.y += GRAVITY_CONSTANT * g_deltaTime;
    p.velocity.x *= 0.99f;
    p.position += p.velocity * g_deltaTime;
    
    g_output[i] = p;
}