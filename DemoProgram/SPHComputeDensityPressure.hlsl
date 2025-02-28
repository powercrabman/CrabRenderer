#include "SPHCommon.hlsli"
#define GRAVITY_CONSTANT (0.098f * 4.f)
#define SMALL_NUMBER (0.0001f)
#define INV_ASPECT_RATIO (1080.f/1920.f)


RWStructuredBuffer<Particle> g_output : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint i = DTid.x;

    uint bufferMaxCount, stride;
    g_output.GetDimensions(bufferMaxCount, stride);

    Particle p = g_output[i];

    if (p.life <= 0.f)
        return;

    float density = 0.f;

    for (uint j = 0; j < bufferMaxCount; j++)
    {
        Particle other = g_output[j];

        if (other.life <= 0.f)
            continue;

        float dist = length(p.position - other.position);

        if (dist >= p.radius)
            continue;

        density += other.mass * CubicSpline(dist * 2.f / p.radius);
    }


    float pressure = stiffness * (pow(abs(density / restDensity), 7.f) - 1.f);

    g_output[i].density = density;
    g_output[i].pressure = pressure;
}