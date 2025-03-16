#include "ShaderCommon.hlsli"

struct PS_Input
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normalW : NORMAL;
    float3 tangentW : TANGENT;
    float3 bitangentW : BITANGENT;
};

// Vertex shader resource
Texture2D g_displacementTex : register(t0);

// Pixel shader resources
Texture2D g_albedoTex : register(t0);
Texture2D g_normalTex : register(t1);
Texture2D g_aoTex : register(t2);
Texture2D g_metallicTex : register(t3);
Texture2D g_roughnessTex : register(t4);
Texture2D g_emissiveTex : register(t5);

// Shadow Map
Texture2D g_basicShadowMap[MAX_LIGHTS] : register(t20); // 20 ~ 23
TextureCube g_omniShadowMap[MAX_LIGHTS] : register(t24); // 24 ~ 27
Texture2DArray g_cascadeShadowMap : register(t28);

static const float3 Fdielectric = 0.04;

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
    float3 diffuse = g_diffuseCube.Sample(g_wrapSampler, N).rgb;
    
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
        
    const float3 Fdielectric = 0.04;
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, pixelToEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo;

    float D = NdfGGX(NdotH, roughness);
    float3 G = SchlickGGX(NdotI, NdotO, roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);
    
    return (diffuseBRDF + specularBRDF) * NdotI;
}

float Texture2DPCF(float2 in_uv, float in_NDCz, float in_radius, Texture2D in_shadowMap)
{
    float shadowFactor = 0.f;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float2 offset = poissonDisk2D[j] * in_radius;
        shadowFactor += in_shadowMap.SampleCmpLevelZero(g_shadowSampler,
                                                           in_uv + offset,
                                                           in_NDCz).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;

}

float Texture2DArrayPCF(float2 in_uv, uint in_index, float in_depth, float in_radius, Texture2DArray in_shadowMaps)
{
    
    float shadowFactor = 0.f;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float2 offset = poissonDisk2D[j] * in_radius;
        shadowFactor += in_shadowMaps.SampleCmpLevelZero(g_shadowSampler,
                                                           float3(in_uv + offset, in_index),
                                                           in_depth).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;
}

float TextureCubePCF(float3 in_lightPosW,
                     float3 in_pixelPosW,
                     float3 in_lightDirection,
                     float in_lightFallOffEnd,
                     float in_radius,
                     TextureCube in_shadowMap)
{
    float shadowFactor = 0.f;

    float lenToLight = distance(in_pixelPosW, in_lightPosW) / in_lightFallOffEnd;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float3 offset = poissonDisk3D[j] * in_radius;
        shadowFactor += in_shadowMap.SampleCmpLevelZero(g_shadowSampler,
                                                        -in_lightDirection + offset,
                                                        lenToLight).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;
}

void ComputeLight(
    float3 in_posW,
    Light in_light,
    uint in_index,
    out float3 L,
    out float3 radiance,
    out float shadowFactor)
{
    L = float3(0.f, 0.f, 0.f);
    radiance = float3(0.f, 0.f, 0.f);

    // Compute Light
    L = in_light.lightType == LIGHT_DIRECTIONAL
        ? -in_light.lightDirection
        : normalize(in_light.lightPosition - in_posW);

    float dist = distance(in_light.lightPosition, in_posW);

    float attenuation = in_light.lightType == LIGHT_DIRECTIONAL
                      ? 1.f
                      : ComputeAttenuation(dist, in_light.fallOffStart, in_light.fallOffEnd);

    // https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/light-types
    float spotAngleCos = dot(-L, normalize(in_light.lightDirection));
    float cosInnerCone = cos(in_light.innerConeAngle * 0.5f);
    float cosOuterCone = cos(in_light.outerConeAngle * 0.5f);
    float spotFactor = in_light.lightType == LIGHT_SPOT
                     ? (spotAngleCos - cosOuterCone) / (cosInnerCone - cosOuterCone)
                     : 1.0;

    radiance = in_light.lightRadiance * in_light.lightStrength
             * attenuation * spotFactor;

    radiance = in_light.lightType == LIGHT_NONE
             ? float3(0.f, 0.f, 0.f) : radiance;

    // Compute Shadow
    shadowFactor = 1.f;

    if (in_light.lightType == LIGHT_POINT)
    {
     //   float lenToLight = distance(in_posW, in_light.lightPosition) / in_light.fallOffEnd;
     //
     //   [unroll]
     //   for (int j = 0; j < 64; j++)
     //   {
     //       float3 offset = g_diskSamples643D[j];
     //       offset.x *= dx;
     //       offset.y *= dy;
     //       offset.z *= dz;
     //       percentLit += g_omniShadowMap[in_index].SampleCmpLevelZero(g_shadowSampler,
     //                                                  -L + offset,
     //                                                  lenToLight).r;
     //   }
   
    }
    else if (in_light.lightType == LIGHT_DIRECTIONAL)
    {
        // for debug
        static const float3 cascadeColor[4] =
        {
            float3(1.0f, 0.0f, 0.0f),
            float3(0.0f, 1.0f, 0.0f),
            float3(0.0f, 0.0f, 1.0f),
            float3(1.0f, 1.0f, 0.0f)
        };

        static const float cascadeRadius[4] = { 3.0, 2.0, 1.0, 0.5 }; // heuristic

        // Cascade Index 결정
        float zInView = mul(float4(in_posW, 1.0), g_view).z;

        float4 cascadeChecks = float4(
                zInView < cascadeRange1,
                zInView < cascadeRange2,
                zInView < cascadeRange3,
                1.0f);

        int cascadeIndex = 4 - dot(cascadeChecks, float4(1, 1, 1, 1)); // 0 ~ 3
        //radiance += cascadeColor[cascadeIndex]; // for debug

        // Light 관점 Depth 결정
        float4 lightScreen = mul(float4(in_posW, 1.0), cascadeShadowViewProj[cascadeIndex]);
        lightScreen.xyz /= lightScreen.w;

        float2 lightTexCoord;
        lightTexCoord.x = 0.5 * lightScreen.x + 0.5;
        lightTexCoord.y = -0.5 * lightScreen.y + 0.5;

        // 최종 depth
        float bias = lerp(0.00005f, 0.f, lightScreen.z); // todo
        float depth = lightScreen.z + bias;

        // 샘플링 크기 설정
        uint width, height, elem, numberOfLevels;
        g_cascadeShadowMap.GetDimensions(0, width, height, elem, numberOfLevels);

        shadowFactor = Texture2DArrayPCF(lightTexCoord, cascadeIndex, depth, cascadeRadius[cascadeIndex] / width, g_cascadeShadowMap);
    }
    else
    {
      // float4 lightScreen = mul(float4(in_posW, 1.0), basicShadowViewProj[in_index]);
      // lightScreen.xyz /= lightScreen.w;
      //
      // float2 lightTexCoord; 
      // lightTexCoord.x = 0.5 * lightScreen.x + 0.5;
      // lightTexCoord.y = -0.5 * lightScreen.y + 0.5;
      //
      // float bias = lerp(0.01f, 0.f, lightScreen.z);
      // float depth = lightScreen.z + bias;
      //
      // [unroll]
      // for (int j = 0; j < 64; j++)
      // {
      //     float2 offset = g_diskSamples642D[j];
      //     offset.x *= dx;
      //     offset.y *= dy;
      //     percentLit += g_basicShadowMap[in_index].SampleCmpLevelZero(g_shadowSampler,
      //                                                lightTexCoord.xy + offset,
      //                                                depth).r;
      // }
   
    }

    shadowFactor = in_light.useShadow
             ? shadowFactor
             : 1.f;
}

//===================================================
// Vertex Shader
//===================================================

PS_Input VSmain(VS_Input input)
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

//===================================================
// Pixel Shader
//===================================================

float4 PSmain(PS_Input input) : SV_TARGET
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
        ComputeLight(input.posW, g_light[i], i, L, radiance, shadowFactor);

        directLighting += DirectBRDF(L, E, input.posW, N, albedo, metallic, roughness) * radiance * shadowFactor;
        directLighting = saturate(directLighting);
    }

    float4 color = float4(ambientLighting + directLighting + emission, g_alpha);
 //   float4 color = float4(directLighting, g_alpha);
    color = clamp(color, 0.0, 1000.0);

    return color;
}
