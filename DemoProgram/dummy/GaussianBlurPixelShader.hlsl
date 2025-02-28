#include "PostProcessingCommon.hlsli"

static const float g_gaussianFilter[7][7] =
{
    { 1 / 4096.0f, 6 / 4096.0f, 15 / 4096.0f, 20 / 4096.0f, 15 / 4096.0f, 6 / 4096.0f, 1 / 4096.0f },
    { 6 / 4096.0f, 36 / 4096.0f, 90 / 4096.0f, 120 / 4096.0f, 90 / 4096.0f, 36 / 4096.0f, 6 / 4096.0f },
    { 15 / 4096.0f, 90 / 4096.0f, 225 / 4096.0f, 300 / 4096.0f, 225 / 4096.0f, 90 / 4096.0f, 15 / 4096.0f },
    { 20 / 4096.0f, 120 / 4096.0f, 300 / 4096.0f, 400 / 4096.0f, 300 / 4096.0f, 120 / 4096.0f, 20 / 4096.0f },
    { 15 / 4096.0f, 90 / 4096.0f, 225 / 4096.0f, 300 / 4096.0f, 225 / 4096.0f, 90 / 4096.0f, 15 / 4096.0f },
    { 6 / 4096.0f, 36 / 4096.0f, 90 / 4096.0f, 120 / 4096.0f, 90 / 4096.0f, 36 / 4096.0f, 6 / 4096.0f },
    { 1 / 4096.0f, 6 / 4096.0f, 15 / 4096.0f, 20 / 4096.0f, 15 / 4096.0f, 6 / 4096.0f, 1 / 4096.0f }
};

float4 main(PSInput input) : SV_TARGET
{
    float4 color = float4(0.f, 0.f, 0.f, 0.f);

    for (int i = 0; i < 7; ++i)
    {
        float2 texCoord;
        texCoord.y = input.texCoord.y + (i - 3) * g_dy;
        
        for (int j = 0; j < 7; ++j)
        {
            texCoord.x = input.texCoord.x + (j - 3) * g_dx;
            
            color += g_texture.Sample(g_sampler, texCoord) * g_gaussianFilter[i][j];
        }
    }
    
    color.w = 1.f;
    return color;
}
