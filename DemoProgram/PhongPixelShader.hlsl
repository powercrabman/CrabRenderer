#include "LightDemoCommon.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 PSmain(PSInput input) : SV_TARGET
{
    float dist = length(g_lightPosW - input.posW);
    float attanuation = CalcAttenuation(dist, g_falloffStart, g_falloffEnd);
    
    // if the distance is greater than the falloff end, the light is not visible
    if (dist > g_falloffEnd)
        attanuation = 0.f;

    // Diffuse
    float3 L = (g_lightPosW - input.posW) / dist;
    float NdotL = max(dot(input.normalW, L), 0.0f);
    float4 diffuse = g_material.diffuse * NdotL;
    
    // Ambient 
    float4 ambient = g_material.ambient;
    
    // Specular 
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    float3 E = normalize(g_eyePosW - input.posW);
    float3 R = reflect(-L, input.normalW);
    float EdotR = max(dot(E, R), 0.0f);
    float specFactor = pow(EdotR, g_material.shininess);
    specular = g_material.specular * specFactor;
    
    float4 finalColor = ambient + (diffuse + specular) * attanuation * g_lightIntensity;
    finalColor *= float4(g_lightColor, 1.f);

    // smooth step
    finalColor = smoothstep(0.f, 1.f, finalColor) * g_useSmoothStep
               + finalColor * (1.f - g_useSmoothStep);
    
    // texture
    float4 texColor = g_texture.Sample(g_sampler, input.texCoord) * g_useTexture
                    + float4(1.f, 1.f, 1.f, 1.f) * (1.f - g_useTexture);
    
    // fianl color
    return texColor * finalColor;
}