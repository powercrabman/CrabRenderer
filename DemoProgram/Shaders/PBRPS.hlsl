#include "ShaderCommon.hlsli"

static const float3 Fdielectric = 0.04;

Texture2D g_albedoTex : register(t0);
Texture2D g_normalTex : register(t1);
Texture2D g_aoTex : register(t2);
Texture2D g_metallicTex : register(t3);
Texture2D g_roughnessTex : register(t4);
Texture2D g_emissiveTex : register(t5);
Texture2D g_shadowMapTex[MAX_LIGHTS] : register(t20);

float3 SchlickFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
}

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

float3 DiffuseIBL(float3 albedo,
                  float3 N,
                  float3 E,
                  float metallic)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(N, E)));
    float3 kd = lerp(1.0 - F, 0.0, metallic);
    float3 diffuse = g_irradiateCube.Sample(g_wrapSampler, N).rgb;
    
    return kd * albedo * diffuse;
}

float3 SpecularIBL(float3 albedo,
                   float3 N,
                   float3 E,
                   float3 R,
                   float metallic,
                   float roughness)
{
    float2 specularBRDF = g_brdfTex.SampleLevel(g_clampSampler, float2(dot(N, E), 1.0 - roughness), 0.0f).rg;
    float3 specularIrradiance = g_specularCube.SampleLevel(g_wrapSampler, R, roughness * 5.0f).rgb;
    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 AmbientLightingByIBL(float3 albedo, float3 N, float3 E, float ao,
                            float metallic, float roughness)
{
    float3 R = normalize(reflect(-E, N));
    float3 diffuseIBL = DiffuseIBL(albedo, N, E, metallic);
    float3 specularIBL = SpecularIBL(albedo, N, E, R, metallic, roughness);
    
    return (diffuseIBL + specularIBL) * ao;
}

float NdfGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
    denom = max(denom, 1e-5);

    return alphaSq / (3.141592 * denom * denom);
}

float SchlickG1(float NdotV, float k)
{
    return NdotV / (NdotV * (1.0 - k) + k);
}

float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
}

float random(float3 seed, int i)
{
    float4 seed4 = float4(seed, i);
    float dot_product = dot(seed4, float4(12.9898, 78.233, 45.164, 94.673));
    return frac(sin(dot_product) * 43758.5453);
}

float N2V(float ndcDepth, matrix invProj)
{
    float4 pointView = mul(float4(0, 0, ndcDepth, 1), invProj);
    return pointView.z / pointView.w;
}

float3 DirectBRDF(float3 lightVec, float3 pixelToEye, float3 posWorld, float3 normalWorld, float3 albedo, float metallic, float roughness)
{
    float3 halfway = normalize(pixelToEye + lightVec);
        
    float NdotI = max(0.0, dot(normalWorld, lightVec));
    float NdotH = max(0.0, dot(normalWorld, halfway));
    float NdotO = max(0.0, dot(normalWorld, pixelToEye));
        
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, pixelToEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo;

    float D = NdfGGX(NdotH, roughness);
    float3 G = SchlickGGX(NdotI, NdotO, roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);
    
    return (diffuseBRDF + specularBRDF) * NdotI;
}

void ComputeLight(
    float3 in_posW,
    LightAttribute in_lightAtt,
    LightTransform in_lightTrans,
    uint in_index,
    out float3 L,
    out float3 radiance,
    out float shadowFactor)
{
    L = float3(0.f, 0.f, 0.f);
    radiance = float3(0.f, 0.f, 0.f);

    // Compute Light
    L = MatchLightType(in_lightAtt.lightType, LIGHT_DIRECTIONAL)
        ? -in_lightTrans.lightDirection
        : normalize(in_lightTrans.lightPosition - in_posW);

    float dist = distance(in_lightTrans.lightPosition, in_posW);

    float attenuation = MatchLightType(in_lightAtt.lightType, LIGHT_DIRECTIONAL)
                      ? ComputeAttenuation(dist, in_lightAtt.fallOffStart, in_lightAtt.fallOffEnd)
                      : 1.f;

    float spotFactor = MatchLightType(in_lightAtt.lightType, LIGHT_SPOT)
                     ? pow(max(0, dot(normalize(in_lightTrans.lightDirection), -L)), in_lightAtt.spotPower)
                     : 1.0;

    radiance = in_lightAtt.lightRadiance * in_lightAtt.lightStrength
             * attenuation * spotFactor;

    radiance = MatchLightType(in_lightAtt.lightType, LIGHT_NONE)
             ? float3(0.f, 0.f, 0.f) : radiance;

    // Compute Shadow (Now, only support for Directional Light)
    shadowFactor = 1.f;

    float4 lightScreen = mul(float4(in_posW, 1.0), in_lightTrans.lightViewProj);
    lightScreen.xyz /= lightScreen.w;

    float2 lightTexCoord;
    lightTexCoord.x = 0.5 * lightScreen.x + 0.5;
    lightTexCoord.y = -0.5 * lightScreen.y + 0.5;

    uint width, height;
    g_shadowMapTex[in_index].GetDimensions(width, height);
    float dx = 5.0 / width;
    float dy = 5.0 / height;
    float percentLit = 0.0;

    [unroll]
    for (int j = 0; j < 64; j++)
    {
        float2 offset = g_diskSamples64[j];
        offset.x *= dx;
        offset.y *= dy;
        percentLit += g_shadowMapTex[in_index].SampleCmpLevelZero(g_shadowSampler, lightTexCoord + offset, lightScreen.z - 0.001f).r;
    }

    shadowFactor = percentLit / 64.f;

    shadowFactor = MatchLightType(in_lightAtt.lightType, LIGHT_NONE)
                 ? 1.f
                 : shadowFactor;
}

float4 main(PS_Input input) : SV_TARGET
{
    float3 E = normalize(g_eyePosW - input.posW);
    float3 N = GetNormal(input.tangentW, input.bitangentW, input.normalW, input.texCoord);
    
    float3 albedo = (g_usingTextureFlags & USING_TEXTURE_BASECOLOR)
                  ? g_albedoTex.Sample(g_wrapSampler, input.texCoord).rgb * g_baseColor
                  : g_baseColor;

    float ao = (g_usingTextureFlags & USING_TEXTURE_AMBIENT_OCCLUSION)
             ? g_aoTex.Sample(g_wrapSampler, input.texCoord).r
             : 1.0;

    float metallic = (g_usingTextureFlags & USING_TEXTURE_METALLIC)
                   ? g_metallicTex.Sample(g_wrapSampler, input.texCoord).r * g_metallic
                   : g_metallic;

    float roughness = (g_usingTextureFlags & USING_TEXTURE_ROUGHNESS)
                    ? g_roughnessTex.Sample(g_wrapSampler, input.texCoord).r * g_roughness
                    : g_roughness;

    float3 emission = (g_usingTextureFlags & USING_TEXTURE_EMISSIVE)
                    ? g_emissiveTex.Sample(g_wrapSampler, input.texCoord).rgb
                    : g_emissive;

    float3 ambientLighting = AmbientLightingByIBL(albedo, N, E, ao, metallic, roughness);

    float3 directLighting = float3(0.0, 0.0, 0.0);

    [unroll]
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        float3 L;
        float3 radiance;
        float shadowFactor;
        ComputeLight(input.posW, g_lightAttribute[i], g_lightTransform[i], i, L, radiance, shadowFactor);

        directLighting += DirectBRDF(L, E, input.posW, N, albedo, metallic, roughness) * radiance * shadowFactor;

    }

    float4 color = float4(ambientLighting + directLighting + emission, g_alpha);
    color = clamp(color, 0.0, 1000.0);
    
    return color;
}
