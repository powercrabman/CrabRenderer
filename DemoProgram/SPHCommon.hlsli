struct Particle
{
    float2 position;
    float2 velocity;
    float3 color;
    float radius;
    float life;

    // for SPH
    float mass;
    float density; // rho
    float pressure; // p
    float2 force;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    uint primID : PRIMITIVEID;
    float life : USERVALUE1;
};

struct GS_Input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float radius : USERVALUE0;
    float life : USERVALUE1;
};

cbuffer ParticleConstant : register(b0)
{
    float deltaTime; // 4
    float restDensity; // p0
    float stiffness; // k
    float viscosity; // u
};

#define PI (3.14159265359f)

float CubicSpline(float q)
{
    float constant = 3.f / (2.f * PI);
    float result = 0.f;

    if (q >= 0.f && q < 1.f)
    {
        // 0.66667 = 2 / 3
        result = 0.666667 - q * q + 0.5 * q * q * q;
    }
    else if (q >= 1.f && q < 2.f)
    {
        // 0.166667 = 1/ 6
        float q2 = 2.f - q;
        result = 0.166667 * q2 * q2 * q2;
    }

    return constant * result;
}

float CubicSplineGrad(float q)
{
    float constant = 3.f / (2.f * PI);
    float result = 0.f;

    if (q >= 0.f && q < 1.f)
    {
        result = -2.f * q + 1.5 * q * q;
    }
    else if (q >= 1.f && q < 2.f)
    {

        result = -0.5 * (2.f - q) * (2.f - q);
    }

    return constant * result;
}

