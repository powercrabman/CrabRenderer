#include "LightDemoCommon.hlsli"

PSInput main(VSInput input)
{
    PSInput output;
    output.color = input.color;
    output.texCoord = input.texCoord;

    output.normalW = mul(float4(input.normalL, 0.f), g_worldInvTranspose).xyz;
    output.normalW = normalize(output.normalW);
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz 
                + output.normalW * input.texCoord.x;
    
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);
    return output;
}