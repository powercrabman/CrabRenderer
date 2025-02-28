#include "PostProcessCommon.hlsli"

cbuffer CombinePSConstant : register(b0)
{
    float g_bloomStrength;
};

Texture2D g_inputTexture1 : register(t0);
Texture2D g_inputTexture2 : register(t1);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 tex1Color = g_inputTexture1.Sample(g_sampler, input.texCoord);
    float4 tex2Color = g_inputTexture2.Sample(g_sampler, input.texCoord);
    float4 finalColor = tex2Color + tex1Color * g_bloomStrength;
    return finalColor;
}

