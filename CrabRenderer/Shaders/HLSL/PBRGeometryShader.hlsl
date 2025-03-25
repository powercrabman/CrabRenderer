#include "ShaderCommon.hlsli"

struct PS_Output
{
    float4 positionW : SV_TARGET0;
    float4 normalW : SV_TARGET1;
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
Texture2D DisplacementTexture : register(t0);

// in pixel shader
Texture2D AlbedoTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D AOTexture : register(t2);
Texture2D MetallicTexture : register(t3);
Texture2D RoughnessTexture : register(t4);
Texture2D EmissiveTexture : register(t5);

//===================================================
// Vertex Shader
//===================================================

PS_Input VSmain(VS_3D_INPUT input)
{
    PS_Input output;

    float heightFactor = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_DISPLACEMENT) ?
        ((DisplacementTexture.SampleLevel(SamplerLinearWrap, input.texCoord, 0.f).r - 0.5f) * 2.0f * cb_materialDisplacementStrength)
        : 0.0f;

    float3 localPos = input.positionL + input.normalL * heightFactor;

    output.posW = mul(float4(localPos, 1.0f), cb_transformWorld).xyz;
    output.posH = mul(float4(output.posW, 1.0f), cb_cameraViewProj);

    output.normalW = normalize(mul(float4(input.normalL, 0.0f), cb_transformWorldInvTranspose).xyz);
    output.tangentW = normalize(mul(float4(input.tangentL, 0.0f), cb_transformWorld).xyz);
    output.bitangentW = normalize(cross(output.normalW, output.tangentW));

    output.texCoord = input.texCoord;

    return output;
}

//===================================================
// Pixel Shader
//===================================================

PS_Output PSmain(PS_Input input)
{
    PS_Output output;
    output.positionW = float4(input.posW, 1.f);
    output.normalW = float4(GetNormal(input.tangentW,
                                      input.bitangentW,
                                      input.normalW,
                                      input.texCoord,
                                      cb_materialTextureFlags,
                                      NormalTexture,
                                      SamplerLinearWrap), 0.f);

    float3 albedo = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_ALBEDO)
                 ? AlbedoTexture.Sample(SamplerLinearWrap, input.texCoord).rgb * cb_materialAlbedo
                 : cb_materialAlbedo;
  
    float ao = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_AO)
            ? AOTexture.Sample(SamplerLinearWrap, input.texCoord).r
            : 1.0;
  
    float metallic = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_METALLIC)
                  ? MetallicTexture.Sample(SamplerLinearWrap, input.texCoord).r * cb_materialMetallic
                  : cb_materialMetallic;
  
    float roughness = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_ROUGHNESS)
                   ? RoughnessTexture.Sample(SamplerLinearWrap, input.texCoord).r * cb_materialRoughness
                   : cb_materialRoughness;
  
    float3 emission = (cb_materialTextureFlags & MATERIAL_TEXTURE_FLAGS_EMISSIVE)
                   ? EmissiveTexture.Sample(SamplerLinearWrap, input.texCoord).rgb
                   : cb_materialEmissive;

    output.albedoRoughness = float4(albedo, roughness);
    output.emissive = float4(emission, 1.f);
    output.metallicAO = float4(metallic, ao, 0.f, 0.f);

    return output;
}