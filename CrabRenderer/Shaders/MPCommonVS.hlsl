#include "MousePickingDemoCommon.hlsli"

PSInput main(VSInput input)
{
    PSInput output;
    output.color = input.color;
    output.texCoord = input.texCoord;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.normalW = normalize(mul(float4(input.normalL, 1.f), g_worldInvTranspose)).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);

    return output;
}