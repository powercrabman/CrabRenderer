#include "ShaderCommon.hlsli"

Texture2D g_normalTex : register(t1);

float3 GetNormal(float3 tangent, float3 bitangent, float3 normal, float2 texCoord)
{
    float3 output = normal;
    
    if (g_usingTextureFlags & (USING_TEXTURE_NORMAL_GL | USING_TEXTURE_NORMAL_DX))
    {
        float3 normalTex = g_normalTex.Sample(g_wrapSampler, texCoord).rgb;
        normalTex = normalize(2.0 * normalTex - 1.0);

        normalTex.y = (g_usingTextureFlags & USING_TEXTURE_NORMAL_GL) ? -normalTex.y : normalTex.y;

        float3x3 TBN = float3x3(tangent, bitangent, normal);
        output = normalize(mul(normalTex, TBN));
    }
    
    return output;
}

float4 main(PS_Input input) : SV_TARGET
{
    float3 normalW = GetNormal(input.tangentW, input.bitangentW, input.normalW, input.texCoord);
    float3 norm = normalize((normalW / 2.f) + 0.5f);
    return float4(norm, 1.0f);
}