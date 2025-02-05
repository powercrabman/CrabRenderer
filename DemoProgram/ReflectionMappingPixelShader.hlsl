#define MATRIX row_major float4x4

struct PSInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normalW : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    float3 pad;
};

struct LightComponent
{
    // 12 + 4
    float3 lightColor;
    float lightIntensity;
    
    // 12 + 4
    float3 direction;
    float fallOffStart;
   
    // 12 + 4
    float3 position;
    float fallOffEnd;

    // 4 + 12(pad)
    float spotPower;
    float3 pad;
};

cbuffer CB_Object : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;
    Material g_material;
    bool g_useTexture;
}

cbuffer CB_Camera : register(b1)
{
    MATRIX viewProj;
    float3 eyePosW;
}

cbuffer CB_PixelShader : register(b2)
{
    LightComponent g_dirLight;
    LightComponent g_pointLight;
    LightComponent g_spotLight;
    bool g_useReflection;
    float g_reflectionStrength;
    bool g_useSmoothStep;
}

Texture2D g_texture : register(t0);
TextureCube g_reflectionTexture : register(t1);
SamplerState g_sampler : register(s0);

float ComputeAttenuation(float in_distance, float in_fallOffStart, float in_fallOffEnd)
{
    return saturate((in_fallOffEnd - in_distance) / (in_fallOffEnd - in_fallOffStart));
}

float4 BlinnPhong(float4 in_lightColor, float in_lightIntensity, float in_attenuation, float3 L, float3 E, float3 N)
{
    float3 H = normalize(L + E);
    float NdotH = max(dot(N, H), 0.0f);
    float specFactor = pow(NdotH, g_material.shininess);
    
    float NdotL = max(dot(N, L), 0.0f);
    float4 diffuse = g_material.diffuse * NdotL;
    float4 specular = g_material.specular * specFactor;
    float4 ambient = g_material.ambient;
    float4 finalColor = ambient + in_lightIntensity * in_attenuation * (diffuse + specular);
    finalColor *= in_lightColor;
    finalColor = saturate(finalColor);
    
    return finalColor;
}

float4 ComputePointLight(LightComponent in_light, float3 in_pos, float3 E, float3 N)
{
    float distance = length(in_light.position - in_pos);
    float3 L = (in_light.position - in_pos) / distance;
    float attenuation = ComputeAttenuation(distance, in_light.fallOffStart, in_light.fallOffEnd);

    // if the distance is greater than the falloff end, the light is not visible
    if (distance > in_light.fallOffEnd)
        attenuation = 0.f;
    
    return BlinnPhong(float4(in_light.lightColor, 1.f),
                          in_light.lightIntensity,
                          attenuation,
                          L, E, N);
}

float4 ComputeDirLight(LightComponent in_light, float3 in_pos, float3 E, float3 N)
{
    float3 L = normalize(-in_light.direction);
    float attenuation = 1.0f;
    
    return BlinnPhong(float4(in_light.lightColor, 1.f),
                      in_light.lightIntensity,
                      attenuation,
                      L, E, N);
}

float4 ComputeSpotLight(LightComponent in_light, float3 in_pos, float3 E, float3 N)
{
    float distance = length(in_light.position - in_pos);
    float3 L = (in_light.position - in_pos) / distance;
    float attenuation = ComputeAttenuation(distance, in_light.fallOffStart, in_light.fallOffEnd);
    
    // if the distance is greater than the falloff end, the light is not visible
    if (distance > in_light.fallOffEnd)
    {
        attenuation = 0.f;
    }

    float spotFactor = max(dot(-L, in_light.direction), 0.f);
    spotFactor = pow(spotFactor, in_light.spotPower);
    
    return BlinnPhong(float4(in_light.lightColor, 1.f),
                          in_light.lightIntensity,
                          attenuation * spotFactor,
                          L, E, N);
}

float4 main(PSInput input) : SV_TARGET
{
    float3 E = normalize(eyePosW - input.posW);
    float3 L = normalize(input.normalW);

    float4 lighting = ComputeDirLight(g_dirLight, input.posW, E, L)
                    + ComputePointLight(g_pointLight, input.posW, E, L)
                    + ComputeSpotLight(g_spotLight, input.posW, E, L);
    lighting = saturate(lighting);
    
    lighting = lerp(lighting, smoothstep(0.0f, 1.0f, lighting), g_useSmoothStep);

    float4 texColor = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f),
                           g_texture.Sample(g_sampler, input.texCoord),
                           g_useTexture);

    float3 R = reflect(-E, L);
    float4 reflSample = g_reflectionTexture.Sample(g_sampler, R);
    float4 reflectionColor = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f),
                                  reflSample,
                                  g_useReflection);
    
    float4 baseColor = lighting * texColor;

    float4 finalColor = lerp(baseColor, baseColor * reflectionColor, g_reflectionStrength);
    finalColor = saturate(finalColor);
    
    return finalColor;
}