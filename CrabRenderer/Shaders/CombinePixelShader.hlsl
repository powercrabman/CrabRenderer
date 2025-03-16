#include "PostProcessingCommon.hlsli"

Texture2D g_originalTexture : register(t1);

float4 main(PSInput input) : SV_TARGET
{
    return g_originalTexture.Sample(g_sampler, input.texCoord) +
    g_texture.Sample(g_sampler, input.texCoord) * g_strength;
}