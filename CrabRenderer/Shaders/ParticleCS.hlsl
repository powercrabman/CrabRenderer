#include "ParticleCommon.hlsli"

#define GRAVITY_CONSTANT (0.98f * 2.f)
#define SMALL_NUMBER (0.0001f)
#define FLOOR_POSITION_Y (-1.f)
#define LEFT_WALL_POSITION_X (-1.f)
#define RIGHT_WALL_POSITION_X (1.f)
#define ATTENUATION_FACTOR (0.8f)
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

    p.life += -0.5f * g_deltaTime;

    p.velocity.y -= GRAVITY_CONSTANT * g_deltaTime;
    p.position += p.velocity * g_deltaTime;
    
    float collisionRegionFloor = FLOOR_POSITION_Y + p.radius;
    if (p.position.y < collisionRegionFloor)
    {
        p.velocity.y = -p.velocity.y * ATTENUATION_FACTOR;
        p.position.y = collisionRegionFloor + SMALL_NUMBER;
        p.velocity.x *= ATTENUATION_FACTOR;
    }

    float collisionRegionLeftWall = LEFT_WALL_POSITION_X + p.radius * INV_ASPECT_RATIO;
    if (p.position.x < collisionRegionLeftWall)
    {
        p.velocity.x = -p.velocity.x * ATTENUATION_FACTOR;
        p.position.x = collisionRegionLeftWall + SMALL_NUMBER;
    }

    float collisionRegionRightWall = RIGHT_WALL_POSITION_X - p.radius * INV_ASPECT_RATIO;
    if (p.position.x > collisionRegionRightWall)
    {
        p.velocity.x = -p.velocity.x * ATTENUATION_FACTOR;
        p.position.x = collisionRegionRightWall - SMALL_NUMBER;
    }

    g_output[i] = p;
}