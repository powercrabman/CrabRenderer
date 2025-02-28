#include "ShaderCommon.hlsli"

Texture2D g_displacementTex : register(t0);

PS_Input main(VS_Input input)
{
    PS_Input output;

    float heightFactor = (g_usingTextureFlags & USING_TEXTURE_DISPLACEMENT) ?
        ((g_displacementTex.SampleLevel(g_wrapSampler, input.texCoord, 0.f).r - 0.5f) * 2.0f * g_displacementStrength)
        : 0.0f;

    float3 localPos = input.posL + input.normalL * heightFactor;

    output.posW = mul(float4(localPos, 1.0f), g_world).xyz;
    output.posH = mul(float4(output.posW, 1.0f), g_viewProj);

    output.normalW = normalize(mul(float4(input.normalL, 0.0f), g_worldInvTranspose).xyz);
    output.tangentW = normalize(mul(float4(input.tangentL, 0.0f), g_world).xyz);
    output.bitangentW = normalize(cross(output.normalW, output.tangentW));

    output.texCoord = input.texCoord;

    return output;
}
