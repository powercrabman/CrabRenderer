#include "PostProcessingCommon.hlsli"

static const float g_weight[5] = { 0.0545f, 0.2442f, 0.4026f, 0.2442f, 0.0545f };

float4 main(PSInput input) : SV_TARGET
{
    float4 color = float4(0.f, 0.f, 0.f, 0.f);

    for (int i = 0; i < 5; ++i)
    {
        float2 texCoord;
        texCoord.x = input.texCoord.x;
        texCoord.y = input.texCoord.y + (i - 2) * g_dy;
        
        color += g_texture.Sample(g_sampler, texCoord) * g_weight[i];
    }
    
    color.w = 1.f;
    return color;
}