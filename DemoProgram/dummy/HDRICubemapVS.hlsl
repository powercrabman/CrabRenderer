#include "HDRICubemapCommon.hlsli"

PSInput main(VSInput input)
{
    PSInput output;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.normalW = mul(float4(input.normalL, 1.0f), g_worldInvTranspose).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);
    output.tangentW = mul(float4(input.tangentL, 1.0f), g_world).xyz;
    output.texCoord = input.texCoord;
    return output;
}