#include "EnvmapCommon.hlsli"

PSInput main(VSInput input)
{
    PSInput output;

    float4 posW = mul(float4(input.posL, 1.0f), g_world);
    float3 normalW = normalize(mul(input.normalL, (float3x3) g_worldInvTranspose));

    output.posH = mul(posW, g_viewProj);

    output.posW = posW.xyz;
    output.normalW = normalW;
    output.texCoord = input.texCoord;
    return output;
}