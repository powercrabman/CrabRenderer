#include "PBRCommon.hlsli"

TextureCube g_irradianceIBLTex : register(t0);
TextureCube g_specularIBLTex : register(t1);
Texture2D g_brdfTex : register(t2);
Texture2D g_normalMap : register(t3);
Texture2D g_albedoMap : register(t4);
Texture2D g_roughnessMap : register(t5);
Texture2D g_metallicMap : register(t6);
Texture2D g_aoMap : register(t7);

SamplerState g_linearSampler : register(s0);
SamplerState g_clampSampler : register(s1);

static const float3 Fdielectric = 0.04;
static const float PI = 3.14159265359;

float3 Fresnel(float3 F0, float NdotH)
{
    return F0 + (1.f - F0) * exp2((-5.55473 * NdotH - 6.98316) * NdotH);
}

float3 GetNormal(float3 normalW, float3 tangent, float2 texCoord)
{
    if (g_useNormalMap)
    {
        float3 N = normalize(normalW);
        float3 T = normalize(tangent - dot(tangent, N) * N);
        float3 B = cross(N, T);

        float3 normal = g_normalMap.Sample(g_linearSampler, texCoord).rgb;
        float3x3 TBN = float3x3(T, B, N);
        normal = 2.0 * normal - 1.0;
        normal = normalize(mul(normal, TBN));
        return normal;
    }
    else
    {
        return normalW;
    }
}

float3 DiffuseIBL(float3 albedo, float3 normalW, float3 E, float metallic)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = Fresnel(F0, max(0.0, dot(normalW, E)));
    float3 kd = lerp(1.0 - F, 0.0, metallic);

    float3 irradiance = g_irradianceIBLTex.Sample(g_clampSampler, normalW).rgb;

    return kd * albedo * irradiance;
}

float3 SpecularIBL(float3 albedo, float3 normalW, float3 E, float metallic, float roughness)
{
    float2 specularBRDF = g_brdfTex.Sample(g_clampSampler, float2(max(dot(E, normalW), 0.f), roughness)).rg;

    float3 specularIrradiance = g_specularIBLTex.SampleLevel(g_linearSampler, reflect(-E, normalW), roughness * 5.0f).rgb;
    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 AmbientLightingByIBL(float3 albedo, float3 normalW, float3 pixelToEye, float ao,
                            float metallic, float roughness)
{
    float3 diffuseIBL = DiffuseIBL(albedo, normalW, pixelToEye, metallic);
    float3 specularIBL = SpecularIBL(albedo, normalW, pixelToEye, metallic, roughness);
    
    return (diffuseIBL + specularIBL) * ao;
}

float NdfGGX(float NdotH, float roughness)
{
    float powerAlpha = roughness * roughness;
    return powerAlpha / (PI * exp2(NdotH * NdotH * (powerAlpha - 1) + 1));
}

float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float k = (roughness + 1) * (roughness + 1) / 8;
    float Gv = NdotI / (NdotI * (1 - k) + k);
    float Gl = NdotO / (NdotO * (1 - k) + k);

    return Gv * Gl;
}

float4 main(PSInput input) : SV_TARGET
{
    float3 pixelToEye = normalize(g_eyePosW - input.posW);
    float3 normalWorld = GetNormal(input.normalW, input.tangentW, input.texCoord);

    float3 albedo = g_useAlbedoMap ? g_albedoMap.Sample(g_linearSampler, input.texCoord).rgb 
                                 : g_material.albedo;
    float ao = g_useAOMap ? g_aoMap.SampleLevel(g_linearSampler, input.texCoord, 0.0).r : 1.f;
    float metallic = g_useMetallicMap ? g_metallicMap.Sample(g_linearSampler, input.texCoord).r 
                                    : g_material.metallic;
    float roughness = g_useRoughnessMap ? g_roughnessMap.Sample(g_linearSampler, input.texCoord).r 
                                      : g_material.roughness;

    float3 ambientLighting = AmbientLightingByIBL(albedo, normalWorld, pixelToEye, ao,
                                                  metallic, roughness);

    float3 directLighting = float3(0, 0, 0);

    // 포인트 라이트만 먼저 구현
    {
        float3 L = normalize(light.position - input.posW);
        float3 H = normalize(pixelToEye + L);
        
        float NdotI = max(0.0, dot(normalWorld, L));
        float NdotH = max(0.0, dot(normalWorld, H));
        float NdotO = max(0.0, dot(normalWorld, pixelToEye));
        
        float3 F0 = lerp(Fdielectric, albedo, metallic);
        float3 F = Fresnel(F0, max(0.0, dot(H, pixelToEye)));
        float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
        float3 diffuseBRDF = kd * albedo;

        float D = NdfGGX(NdotH, roughness);
        float3 G = SchlickGGX(NdotI, NdotO, roughness);
        
        float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

        float3 radiance = light.radiance * saturate((light.fallOffEnd - length(light.direction)) / (light.fallOffEnd - light.fallOffStart));

        directLighting += (diffuseBRDF + specularBRDF) * radiance * NdotI;
    }
    
    float4 output;
    output = float4(ambientLighting + directLighting, 1.0);
    output = clamp(output, 0.0, 1000.0);
    
    return output;
}
