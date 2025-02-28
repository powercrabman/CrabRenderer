#include "SPHCommon.hlsli"
#define GRAVITY_CONSTANT (9.8f)
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

    float2 pressureF = float2(0.f, 0.f);
    float2 viscosityF = float2(0.f, 0.f);

    for (uint j = 0; j < bufferMaxCount; j++)
    {
        Particle other = g_output[j];

        if (other.life <= 0.f)
            continue;

        float2 xij = p.position - other.position;
        float dist = length(p.position - other.position);

        if (dist >= p.radius)
            continue;

        if (dist < SMALL_NUMBER)
            continue;

        float2 gradPressure = p.density * p.mass
                            * (p.pressure / (p.density * p.density) + other.pressure / (other.density * other.density))
                            * CubicSplineGrad(dist * 2.f / p.radius) * xij / dist;
        pressureF += gradPressure;

        float2 laplacianVelocity = 2.f * p.mass / p.density * (p.velocity - other.velocity)
                                 / (dist * dist + 0.01f * p.radius * p.radius)
                                 * CubicSplineGrad(dist * 2.f / p.radius) * dot(xij, xij / dist);
        viscosityF += viscosity * laplacianVelocity;
    }


    pressureF *= -p.mass / p.density;
    viscosityF *= p.mass;

    float2 gravityF = float2(0.f, -GRAVITY_CONSTANT);
    g_output[i].force = pressureF + viscosityF + gravityF;

}






