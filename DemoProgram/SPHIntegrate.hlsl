#include "SPHCommon.hlsli"
#define GRAVITY_CONSTANT (0.098f * 2.f)
#define SMALL_NUMBER (0.0001f)
#define INV_ASPECT_RATIO (1080.f/1920.f)
#define ATTENUATION (0.5f)
#define SIMULATION_DELTA (0.001f)

RWStructuredBuffer<Particle> g_output : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint i = DTid.x;
    uint bufferMaxCount, stride;
    g_output.GetDimensions(bufferMaxCount, stride);

    g_output[i].velocity += SIMULATION_DELTA * g_output[i].force / (g_output[i].density + SMALL_NUMBER);
    g_output[i].position += SIMULATION_DELTA * g_output[i].velocity;

    if (g_output[i].position.x < -0.9f)
    {
        g_output[i].position.x = -0.9f;
        g_output[i].velocity.x = -g_output[i].velocity.x * ATTENUATION;
    }
    else if (g_output[i].position.x > 0.9f)
    {
        g_output[i].position.x = 0.9f;
        g_output[i].velocity.x = -g_output[i].velocity.x * ATTENUATION;
    }

    if (g_output[i].position.y < -0.9f)
    {
        g_output[i].position.y = -0.9f;
        g_output[i].velocity.y = -g_output[i].velocity.y * ATTENUATION;
    }
    else if (g_output[i].position.y > 0.9f)
    {
        g_output[i].position.y = 0.9f;
        g_output[i].velocity.y = -g_output[i].velocity.y * ATTENUATION;
    }
}


