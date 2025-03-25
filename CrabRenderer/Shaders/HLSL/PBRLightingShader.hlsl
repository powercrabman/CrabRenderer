#include "ShaderCommon.hlsli"
#include "PBRCommon.hlsli"

struct PS_Input
{
    float4 posH : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

// Pixel shader resources
Texture2D NormalRTTexture : register(t0);
Texture2D PositionWorldRTTexture : register(t1);
Texture2D EmissiveRTTexture : register(t2);
Texture2D AlbedoRoughnessRTTexture : register(t3);
Texture2D MetallicAORTTexture : register(t4);

// Shadow Map
Texture2D       ShadowMapTexture[LIGHT_COUNT] : register(t20);      // 20 ~ 23
TextureCube     OmniShadowCubeTexture[LIGHT_COUNT] : register(t24); // 24 ~ 27
Texture2DArray  CascadeShadowMapTextureArray : register(t28);

float3 DirectBRDF(float3 lightDirection, 
                  float3 toEye, 
                  float3 normal, 
                  float3 albedo, 
                  float  metallic, 
                  float  roughness)
{
    float3 halfway = normalize(toEye+ lightDirection);
        
    float NdotI = max(0.0, dot(normal, lightDirection));
    float NdotH = max(0.0, dot(normal, halfway));
    float NdotO = max(0.0, dot(normal, toEye));
        
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, toEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    float3 diffuseBRDF = kd * albedo;

    float D = NdfGGX(NdotH, roughness);
    float3 G = SchlickGGX(NdotI, NdotO, roughness);
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);
    
    return (diffuseBRDF + specularBRDF) * NdotI;
}

float Texture2DPCF(float2    uv,
                   float     zNDC, 
                   float     radius, 
                   Texture2D shadowMapTexture)
{
    float shadowFactor = 0.f;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float2 offset = poissonDisk2D[j] * radius;
        shadowFactor += shadowMapTexture.SampleCmpLevelZero(SamplerComparisonLinearWrap,
                                                           uv + offset,
                                                           zNDC).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;

}

float Texture2DArrayPCF(float2         uv, 
                        uint           arrayIndex, 
                        float          depth, 
                        float          radius, 
                        Texture2DArray shadowMapTextureArray)
{
    
    float shadowFactor = 0.f;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float2 offset = poissonDisk2D[j] * radius;
        shadowFactor += shadowMapTextureArray.SampleCmpLevelZero(SamplerComparisonLinearWrap,
                                                           float3(uv + offset, arrayIndex),
                                                           depth).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;
}

float TextureCubePCF(float3      lightPositionW,
                     float3      positionW,
                     float3      lightDirection,
                     float       lightFallOffEnd,
                     float       radius,
                     TextureCube shadowMapTextureCube)
{
    float shadowFactor = 0.f;

    float lenToLight = distance(positionW, lightPositionW) / lightFallOffEnd;

    [unroll]
    for (int j = 0; j < PCF_FILTER_SAMPLING_COUNT; j++)
    {
        float3 offset = poissonDisk3D[j] * radius;
        shadowFactor += shadowMapTextureCube.SampleCmpLevelZero(SamplerComparisonLinearWrap,
                                                        -lightDirection + offset,
                                                        lenToLight).r;
    }

    return shadowFactor / PCF_FILTER_SAMPLING_COUNT;
}

void ComputeLight(
    float3       positionW,
    LightSubdata light,
    uint         index,
    out float3   toLight,
    out float3   radiance,
    out float    shadowFactor)
{
    toLight = float3(0.f, 0.f, 0.f);
    radiance = float3(0.f, 0.f, 0.f);

    // Compute Light
    toLight = light.lightType == LIGHT_TYPE_DIRECTIONAL
        ? -light.lightDirection
        : normalize(light.lightPosition - positionW);

    float dist = distance(light.lightPosition, positionW);

    float attenuation = light.lightType == LIGHT_TYPE_DIRECTIONAL
                      ? 1.f
                      : ComputeAttenuation(dist, light.fallOffStart, light.fallOffEnd);

    // https://learn.microsoft.com/en-us/windows/uwp/graphics-concepts/light-types
    float spotAngleCos = dot(-toLight, normalize(light.lightDirection));
    float cosInnerCone = cos(light.lightInnerConeAngle * 0.5f);
    float cosOuterCone = cos(light.lightOuterConeAngle * 0.5f);
    float spotFactor = light.lightType == LIGHT_TYPE_SPOT
                     ? (spotAngleCos - cosOuterCone) / (cosInnerCone - cosOuterCone)
                     : 1.0;

    radiance = light.lightRadiance * light.lightStrength
             * attenuation * spotFactor;

    radiance = light.lightType == LIGHT_TYPE_NONE
             ? float3(0.f, 0.f, 0.f) : radiance;

    // Compute Shadow
    shadowFactor = 1.f;

    if (light.lightType == LIGHT_TYPE_POINT)
    {
     //   float lenToLight = distance(positionW, light.cb_lightPosition) / light.cb_fallOffEnd;
     //
     //   [unroll]
     //   for (int j = 0; j < 64; j++)
     //   {
     //       float3 offset = g_diskSamples643D[j];
     //       offset.x *= dx;
     //       offset.y *= dy;
     //       offset.z *= dz;
     //       percentLit += OmniShadowCubeTexture[index].SampleCmpLevelZero(SamplerComparisonLinearWrap,
     //                                                  -toLight + offset,
     //                                                  lenToLight).r;
     //   }
   
    }
    else if (light.lightType == LIGHT_TYPE_DIRECTIONAL)
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
        float zInView = mul(float4(positionW, 1.0), cb_cameraView).z;

        float4 cascadeChecks = float4(
                zInView < cb_cascadeRange1,
                zInView < cb_cascadeRange2,
                zInView < cb_cascadeRange3,
                1.0f);

        int cascadeIndex = 4 - dot(cascadeChecks, float4(1, 1, 1, 1)); // 0 ~ 3
        //radiance += cascadeColor[cascadeIndex]; // for debug

        // Light 관점 Depth 결정
        float4 lightScreen = mul(float4(positionW, 1.0), cb_cascadeShadowReceiverViewProj[cascadeIndex]);
        lightScreen.xyz /= lightScreen.w;

        float2 lightTexCoord;
        lightTexCoord.x = 0.5 * lightScreen.x + 0.5;
        lightTexCoord.y = -0.5 * lightScreen.y + 0.5;

        // 최종 depth
        float bias = lerp(0.00005f, 0.f, lightScreen.z); // todo
        float depth = lightScreen.z + bias;

        // 샘플링 크기 설정
        uint width, height, elem, numberOfLevels;
        CascadeShadowMapTextureArray.GetDimensions(0, width, height, elem, numberOfLevels);

        shadowFactor = Texture2DArrayPCF(lightTexCoord, cascadeIndex, depth, cascadeRadius[cascadeIndex] / width, CascadeShadowMapTextureArray);
    }
    else
    {
      // float4 lightScreen = mul(float4(positionW, 1.0), basicShadowViewProj[index]);
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
      //     percentLit += ShadowMapTexture[index].SampleCmpLevelZero(SamplerComparisonLinearWrap,
      //                                                lightTexCoord.xy + offset,
      //                                                depth).r;
      // }
   
    }

    shadowFactor = light.shadowMappingEnable
             ? shadowFactor
             : 1.f;
}

//===================================================
// Vertex Shader
//===================================================

PS_Input VSmain(float2 positionL : POSITION,
                float2 texCoord : TEXCOORD)
{
    PS_Input output;
    output.posH = float4(positionL, 0.0f, 1.0f);
    output.texCoord = texCoord;
    return output;
}

//===================================================
// Pixel Shader
//===================================================

float4 PSmain(PS_Input input) : SV_TARGET
{

    float3 positionW = PositionWorldRTTexture.Sample(SamplerPointClamp, input.texCoord).rgb;

    float3 N = NormalRTTexture.Sample(SamplerPointClamp, input.texCoord).rgb;

    float3 emission = EmissiveRTTexture.Sample(SamplerPointClamp, input.texCoord).rgb;

    float3 albedo = AlbedoRoughnessRTTexture.Sample(SamplerPointClamp, input.texCoord).rgb;
    float roughness = AlbedoRoughnessRTTexture.Sample(SamplerPointClamp, input.texCoord).r;

    float ao = MetallicAORTTexture.Sample(SamplerPointClamp, input.texCoord).g;
    float metallic = MetallicAORTTexture.Sample(SamplerPointClamp, input.texCoord).r;

    float3 E = normalize(cb_cameraPosition - positionW);

    float3 ambientLighting = AmbientLightingByIBL(albedo, N, E, ao, metallic, roughness,
                             DifCubemap, SamplerLinearWrap, BRDFTexture, SamplerLinearClamp, SpecCubemap, SamplerLinearWrap);
  
    float3 directLighting = float3(0.0, 0.0, 0.0);
  
    [unroll]
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        float3 L;
        float3 radiance;
        float shadowFactor;
        ComputeLight(positionW, cb_lights[i], i, L, radiance, shadowFactor);

        directLighting += DirectBRDF(L, E, N, albedo, metallic, roughness) * radiance * shadowFactor;
        directLighting = saturate(directLighting);
    }

    float4 color = float4(ambientLighting + directLighting + emission, 1.f); // can not use alpha channel
    // float4 color = float4(directLighting, g_alpha);
    color = clamp(color, 0.0, 1000.0);

    return color;
}
