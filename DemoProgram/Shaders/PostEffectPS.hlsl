#include "ShaderCommon.hlsli"
#include "PostProcessCommon.hlsli"

Texture2D g_inputTexture : register(t0);
Texture2D g_depthOnlyTexture : register(t1);
Texture2D g_originalTexture : register(t2);

SamplerState g_sampler : register(s0);

cbuffer PostEffectConstant : register(b0)
{
    //--------------------------------------
    float3 g_fogColor = float3(1.f, 1.f, 1.f);
    float g_fogStrength;
    //--------------------------------------
    float g_fogFallOffStart = 5.f;
    float g_fogFallOffEnd = 50.f;
};

float3 TexCoordToWorld(float2 in_texCoord)
{
    float4 NDCPos;
    NDCPos.x = in_texCoord.x * 2.0f - 1.0f;
    NDCPos.y = (1.0f - in_texCoord.y) * 2.0f - 1.0f;
    NDCPos.z = g_depthOnlyTexture.Sample(g_sampler, in_texCoord).r;
    NDCPos.w = 1.f;

    float4 worldPos = mul(NDCPos, g_viewProjInv);
    worldPos /= worldPos.w;

    return worldPos.xyz;
}

float4 main(PSInput input) : SV_TARGET
{
    float3 worldPos = TexCoordToWorld(input.texCoord);
    float4 originalTex = g_originalTexture.Sample(g_sampler, input.texCoord);
    float dist = distance(worldPos, g_eyePosW);
    float att = g_fogStrength
              ? ComputeAttenuation(dist, g_fogFallOffStart, g_fogFallOffEnd) / g_fogStrength
              : 1.f;
    
    att = saturate(att);
    return lerp(float4(g_fogColor, 1.f), originalTex, att);
}