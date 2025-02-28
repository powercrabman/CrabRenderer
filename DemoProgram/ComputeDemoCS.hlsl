
RWTexture2D<float4> g_output : register(u0);

static const uint g_width = 1920;
static const uint g_height = 1080;
static const uint2 g_center = uint2(g_width / 2, g_height / 2);
static const uint g_radius = 500;

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID,
          uint3 GTid : SV_GroupThreadID,
          uint3 Gid : SV_GroupID)
{
    uint2 pos = uint2(DTid.x, DTid.y);
    uint distToCenter = distance(pos, g_center);

    if (distToCenter < g_radius)
    {
        g_output[DTid.xy] = float4(1.f, 1.f, 1.f, 1.f);
    }
    else
    {
        g_output[DTid.xy] = float4(0.5f, 0.5f, 0.5f, 1.f);
    }
}