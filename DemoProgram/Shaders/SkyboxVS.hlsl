#include "ShaderCommon.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);

    output.normalW = normalize(mul(float4(input.normalL, 0.0f), g_worldInvTranspose).xyz);
    output.tangentW = normalize(mul(float4(input.tangentL, 0.0f), g_world).xyz);
    output.bitangentW = normalize(cross(output.normalW, output.tangentW));

    output.texCoord = input.texCoord;

    return output;
}
