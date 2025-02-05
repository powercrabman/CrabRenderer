#include "LightDemoCommon.hlsli"

PSInput VSmain(VSInput input)
{
    PSInput output;
    output.color = input.color;
    output.texCoord = input.texCoord;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);
    
    output.normalW = normalize(mul(float4(input.normalL, 0.f), g_worldInvTranspose).xyz);
    
    return output;
}