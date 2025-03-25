static const float3 Fdielectric = 0.04;

float3 SchlickFresnel(float3 F0, float NdotH)
{
    return F0 + (1.0 - F0) * pow(2.0, (-5.55473 * NdotH - 6.98316) * NdotH);
}

float3 DiffuseIBL(float3 albedo,
                  float3 N,
                  float3 E,
                  float metallic,
                  TextureCube diffuseMap,
                  SamplerState diffuseMapSampler)
{
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(N, E)));
    float3 kd = lerp(1.0 - F, 0.0, metallic);
    float3 diffuse = diffuseMap.Sample(diffuseMapSampler, N).rgb;
    
    return kd * albedo * diffuse;
}

float3 SpecularIBL(float3 albedo,
                   float3 N,
                   float3 E,
                   float3 R,
                   float metallic,
                   float roughness,
                   Texture2D BRDFTex,
                   SamplerState BRDFSampler, // expect clamp sampler
                   TextureCube specularCube,
                   SamplerState specularSampler)
{
    float2 specularBRDF = BRDFTex.SampleLevel(BRDFSampler, float2(dot(N, E), 1.0 - roughness), 0.0f).rg;
    float3 specularIrradiance = specularCube.SampleLevel(specularSampler, R, roughness * 5.0f).rgb;
    float3 F0 = lerp(Fdielectric, albedo, metallic);

    return (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;
}

float3 AmbientLightingByIBL(float3 albedo,
                            float3 N,
                            float3 E,
                            float ao,
                            float metallic,
                            float roughness,
                            TextureCube diffuseMap,
                            SamplerState diffuseMapSampler,
                            Texture2D BRDFTex,
                            SamplerState BRDFSampler,
                            TextureCube specularCube,
                            SamplerState specularSampler)
{
    float3 R = normalize(reflect(-E, N));
    float3 diffuseIBL = DiffuseIBL(albedo, N, E, metallic, diffuseMap, diffuseMapSampler);
    float3 specularIBL = SpecularIBL(albedo, N, E, R, metallic, roughness, BRDFTex, BRDFSampler, specularCube, specularSampler);
    
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