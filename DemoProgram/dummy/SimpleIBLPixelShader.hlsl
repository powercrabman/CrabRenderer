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
    float g_diffuseMapStrength;
    float g_specularMapStrength;
    float g_speculerShininess;
    bool g_useSmoothStep;
}

Texture2D g_texture : register(t0);
TextureCube g_diffuseTexture : register(t1);
TextureCube g_specularTexture : register(t2);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float3 E = normalize(eyePosW - input.posW);
    float3 N = normalize(input.normalW);

    float3 diffuse = float3(1.f, 1.f, 1.f);
    float3 specular = float3(1.f, 1.f, 1.f);
    
    // diffuse map
    float3 diffuseTex = g_texture.Sample(g_sampler, input.texCoord).rgb;
    diffuseTex = lerp(float3(1.f, 1.f, 1.f), diffuseTex, g_useTexture);

    float3 diffuseMap = g_diffuseTexture.Sample(g_sampler, input.normalW).rgb * g_material.diffuse.rgb;
    diffuse = lerp(diffuse, diffuseMap, g_diffuseMapStrength);
    diffuse *= diffuseTex;
    
    // specular map
    float3 R = reflect(-E, N);
    float3 specularMap = g_specularTexture.Sample(g_sampler, R).rgb;
    specularMap = pow(abs(specularMap), g_speculerShininess) * g_material.specular.rgb;
    specular = lerp(float3(0.f, 0.f, 0.f), specularMap, g_specularMapStrength);
   
    float4 finalColor = float4(saturate(diffuse + specular), 1.f);
    return finalColor;
}