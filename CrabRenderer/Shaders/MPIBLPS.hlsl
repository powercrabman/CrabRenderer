#include "MousePickingDemoCommon.hlsli"

SamplerState g_sampler : register(s0);

TextureCube g_diffuseTexture : register(t0);
TextureCube g_specularTexture : register(t1);
Texture2D g_texture : register(t2);


struct PSOutput
{
    float4 backbufferColor : SV_TARGET0;
    uint4 entityColor : SV_TARGET1;
};

PSOutput main(PSInput input)
{
    float3 diffuseColor = float3(1.f, 1.f, 1.f);
    float3 specularColor = float3(1.f, 1.f, 1.f);
    
    float3 diffueTexture = lerp(float3(1.f, 1.f, 1.f),
                                g_texture.Sample(g_sampler, input.texCoord).rgb,
                                g_useTexture);

    float3 diffuseMap = lerp(float3(1.f, 1.f, 1.f),
                             g_diffuseTexture.Sample(g_sampler, input.normalW).rgb,
                             g_diffuseMapStrength);

    float3 E = normalize(g_eyePos - input.posW);
    float3 N = normalize(input.normalW);
    float3 R = reflect(-E, N);
    float3 specularMap = lerp(float3(0.f, 0.f, 0.f),
                         g_specularTexture.Sample(g_sampler, R).rgb,
                         g_specularMapStrength);

    specularMap = pow(abs(specularMap), g_speculerShininess);
    
    float3 finalColor = g_diffuse.rgb * diffueTexture * diffuseMap +
                        g_specular.rgb * specularMap;
    
    finalColor = saturate(finalColor);

    PSOutput output;
    output.backbufferColor = float4(finalColor, 1.f);
    output.entityColor = uint4(0, 0, 0, 0);
    
    return output;
}