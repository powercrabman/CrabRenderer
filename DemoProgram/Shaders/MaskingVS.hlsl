#include "ShaderCommon.hlsli" 

PS_Input main(VS_Input input)
{
    PS_Input output;
    output.posW = mul(float4(input.posL, 1.0f), g_world).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);

    output.normalW = 0.f;
    output.tangentW = 0.f;
    output.bitangentW = 0.f;

    output.texCoord = input.texCoord;

    return output;
}
