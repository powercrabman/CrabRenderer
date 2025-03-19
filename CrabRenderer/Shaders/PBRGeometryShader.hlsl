#include "ShaderCommon.hlsli"

struct PS_Output
{
    float4 worldPos : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 emissive : SV_TARGET2;
    float4 albedoRoughness : SV_TARGET3;
    float4 metallicAO : SV_TARGET4;
};

struct PS_Input
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float3 bitangentW : BITANGENT;
};

// in vertex shader
Texture2D g_displacementTex : register(t0);

// in pixel shader
Texture2D g_albedoTex : register(t0);
Texture2D g_normalTex : register(t1);
Texture2D g_aoTex : register(t2);
Texture2D g_metallicTex : register(t3);
Texture2D g_roughnessTex : register(t4);
Texture2D g_emissiveTex : register(t5);

//===================================================
// Vertex Shader
//===================================================

PS_Input VSmain(VS_Input input)
{
    PS_Input output;

    float heightFactor = (g_usingTextureFlags & USING_TEXTURE_DISPLACEMENT) ?
        ((g_displacementTex.SampleLevel(g_linearWrapSampler, input.texCoord, 0.f).r - 0.5f) * 2.0f * g_displacementStrength)
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

//===================================================
// Pixel Shader
//===================================================

PS_Output PSMain(PS_Input input)
{
    PS_Output output = (PS_Output) 0;
    output.worldPos = float4(input.posW, 1.f);
    output.normal = float4(GetNormal(input.tangentW, input.bitangentW, input.normalW, input.texCoord, g_normalTex), 0.f);

    float3 albedo = (g_usingTextureFlags & USING_TEXTURE_BASECOLOR)
                 ? g_albedoTex.Sample(g_linearWrapSampler, input.texCoord).rgb * g_baseColor
                 : g_baseColor;
  
    float ao = (g_usingTextureFlags & USING_TEXTURE_AMBIENT_OCCLUSION)
            ? g_aoTex.Sample(g_linearWrapSampler, input.texCoord).r
            : 1.0;
  
    float metallic = (g_usingTextureFlags & USING_TEXTURE_METALLIC)
                  ? g_metallicTex.Sample(g_linearWrapSampler, input.texCoord).r * g_metallic
                  : g_metallic;
  
    float roughness = (g_usingTextureFlags & USING_TEXTURE_ROUGHNESS)
                   ? g_roughnessTex.Sample(g_linearWrapSampler, input.texCoord).r * g_roughness
                   : g_roughness;
  
    float3 emission = (g_usingTextureFlags & USING_TEXTURE_EMISSIVE)
                   ? g_emissiveTex.Sample(g_linearWrapSampler, input.texCoord).rgb
                   : g_emissive;

    output.albedoRoughness = float4(albedo, roughness);
    output.emissive = float4(emission, 1.f);
    output.metallicAO = float4(metallic, ao, 0.f, 0.f);

    return output;
}