#include "NMDemoCommon.hlsli"

Texture2D g_diffuseTexture : register(t0);
Texture2D g_normalTexture : register(t1);
SamplerState g_sampler : register(s0);

static const float g_radius = 0.25f;
static const float2 g_center = float2(0.5f, 0.5f);


float4 main(PSInput input) : SV_TARGET
{
    float3 L = normalize(g_lightPosW - input.posW);
    float3 N = normalize(input.normalW);

    if (g_useNormalTexture)
    {
        float3 normalTex = g_normalTexture.Sample(g_sampler, input.texCoord).rgb;
        normalTex = 2.0f * normalTex - 1.0f;
        
        float3 T = normalize(input.tangentW - dot(input.tangentW, N) * N);
        float3 B = cross(N, T);
        
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normalTex, TBN));
    }
    
    float LdotN = max(dot(L, N), 0.0f);
    float3 color = float3(1.f, 1.f, 1.f) * LdotN;
    
    if (g_useDiffuseTexture)
    {
        color *= g_diffuseTexture.Sample(g_sampler, input.texCoord).rgb;
    }
  
    return float4(color, 1.0f);
}