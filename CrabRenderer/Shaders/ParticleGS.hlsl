#include "ParticleCommon.hlsli"

const static float2 g_resolution = float2(1920, 1080);
const static float g_aspectRatio = g_resolution.x / g_resolution.y;

const static int g_dx[4] = { -1, 1, 1, -1 };
const static int g_dy[4] = { 1, 1, -1, -1 };

[maxvertexcount(6)]
void main(
    point GS_Input input[1],
    inout TriangleStream<PS_Input> output
)
{
    PS_Input p[4];

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        p[i].pos = input[0].pos;
        p[i].pos.x += g_dx[i] * input[0].radius;
        p[i].pos.y += g_dy[i] * input[0].radius * g_aspectRatio;
        p[i].color = input[0].color;
        p[i].life = input[0].life;
        p[i].texCoord = float2(g_dx[i] * 0.5f + 0.5f, g_dy[i] * 0.5f + 0.5f);
    }

    output.Append(p[0]);
    output.Append(p[1]);
    output.Append(p[2]);
    output.RestartStrip();

    output.Append(p[0]);
    output.Append(p[2]);
    output.Append(p[3]);
}