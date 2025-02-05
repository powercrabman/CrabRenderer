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
    float4 ambiant;
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
    float g_diffuseMapStrength;
    float g_specularMapStrength;
    bool g_useSmoothStep;
}

Texture2D g_texture : register(t0);
TextureCube g_diffuseTexture : register(t1);
TextureCube g_specularTexture : register(t2);
SamplerState g_sampler : register(s0);

float ComputeAttenuation(float in_distance, float in_fallOffStart, float in_fallOffEnd)
{
    return saturate((in_fallOffEnd - in_distance) / (in_fallOffEnd - in_fallOffStart));
}

void BlinnPhong(float3 in_lightColor,
                float in_lightIntensity,
                float in_attenuation,
                float3 L, float3 E, float3 N,
                inout float3 out_ambiant,
                inout float3 out_diffuse,
                inout float3 out_specular)
{
    float3 H = normalize(L + E);
    float NdotH = max(dot(N, H), 0.0f);
    float specFactor = pow(NdotH, g_material.shininess);
    
    float NdotL = max(dot(N, L), 0.0f);

    float attenuationFactor = in_lightIntensity * in_attenuation;
    
    float3 diffuse = g_material.diffuse.rgb * NdotL * in_lightColor * attenuationFactor;
    float3 specular = g_material.specular.rgb * specFactor * in_lightColor * attenuationFactor;
    float3 ambiant = g_material.ambiant.rgb * in_lightColor;
    
    out_ambiant += ambiant;
    out_diffuse += diffuse;
    out_specular += specular;
}

void ComputePointLight(LightComponent in_light,
                         float3 in_pos,
                         float3 E, float3 N,
                         inout float3 out_ambiant,
                         inout float3 out_diffuse,
                         inout float3 out_specular)
{
    float distance = length(in_light.position - in_pos);
    float3 L = (in_light.position - in_pos) / distance;
    float attenuation = ComputeAttenuation(distance, in_light.fallOffStart, in_light.fallOffEnd);

    // if the distance is greater than the falloff end, the light is not visible
    if (distance > in_light.fallOffEnd)
        attenuation = 0.f;
    
    BlinnPhong(in_light.lightColor,
               in_light.lightIntensity,
               attenuation,
               L, E, N,
               out_ambiant,
               out_diffuse,
               out_specular);
}

void ComputeDirLight(LightComponent in_light,
                       float3 in_pos,
                       float3 E, float3 N,
                       inout float3 out_ambiant,
                       inout float3 out_diffuse,
                       inout float3 out_specular)
{
    float3 L = normalize(-in_light.direction);
    float attenuation = 1.0f;
    
    BlinnPhong(in_light.lightColor,
               in_light.lightIntensity,
               attenuation,
               L, E, N,
               out_ambiant,
               out_diffuse,
               out_specular);
}

void ComputeSpotLight(LightComponent in_light,
                        float3 in_pos,
                        float3 E, float3 N,
                        inout float3 out_ambiant,
                        inout float3 out_diffuse,
                        inout float3 out_specular)
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
    
    BlinnPhong(in_light.lightColor,
               in_light.lightIntensity,
               attenuation * spotFactor,
               L, E, N,
               out_ambiant,
               out_diffuse,
               out_specular);
}

float4 main(PSInput input) : SV_TARGET
{
    float3 E = normalize(eyePosW - input.posW);
    float3 L = normalize(input.normalW);

    float3 diffuse = float3(0.f, 0.f, 0.f);
    float3 specular = float3(0.f, 0.f, 0.f);
    float3 ambiant = float3(0.f, 0.f, 0.f);
    
    ComputeDirLight(g_dirLight, input.posW, E, L, ambiant, diffuse, specular);
    //ComputePointLight(g_pointLight, input.posW, E, L, ambiant, diffuse, specular);
    //ComputeSpotLight(g_spotLight, input.posW, E, L, ambiant, diffuse, specular);
    
    //smoothstep
    ambiant = lerp(ambiant, smoothstep(0.0f, 1.0f, ambiant), g_useSmoothStep);
    diffuse = lerp(diffuse, smoothstep(0.0f, 1.0f, diffuse), g_useSmoothStep);
    specular = lerp(specular, smoothstep(0.0f, 1.0f, specular), g_useSmoothStep);

    // diffuse map
    float3 R = reflect(-E, L);
    float3 diffuseMap = g_diffuseTexture.Sample(g_sampler, R).rgb * g_material.diffuse.rgb;
    diffuse = lerp(diffuse, diffuseMap, g_diffuseMapStrength);

    // specular map
    float3 specularMap = g_specularTexture.Sample(g_sampler, R).rgb;
    specular = lerp(specular, specularMap, g_specularMapStrength);

    // base color 
    float4 baseColor = float4(saturate(ambiant + diffuse + specular), 1.f);

    // texture map
    float4 texColor = lerp(float4(1.0f, 1.0f, 1.0f, 1.0f),
                       g_texture.Sample(g_sampler, input.texCoord),
                       g_useTexture);

    float4 finalColor = saturate(baseColor * texColor);
    return finalColor;
}