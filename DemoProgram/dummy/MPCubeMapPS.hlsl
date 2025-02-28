#include "MousePickingDemoCommon.hlsli"

TextureCube g_cubeMapTexture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    return g_cubeMapTexture.Sample(g_sampler, input.posW);
}