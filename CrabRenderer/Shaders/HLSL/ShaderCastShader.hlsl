#include "ShaderCommon.hlsli"

struct GS_Input
{
    float4 posW : SV_POSITION;
};

struct PS_Input
{
    float4 posH : SV_POSITION;

#ifndef BASIC_SHADOW
    float3 posW : POSITION;
    uint rtIndex : SV_RenderTargetArrayIndex;
#endif
};


#ifdef BASIC_SHADOW
PS_Input VSmain(VS_3D_INPUT input)
{
    PS_Input output;
    float4 posW = mul(float4(input.positionL, 1.f), cb_transformWorld);
    output.posH = mul(posW, cb_shadowCasterViewProj);
    return output;
}
#else
GS_Input VSmain(VS_3D_INPUT input)
{
    GS_Input output;
    output.posW = mul(float4(input.positionL, 1.f), cb_transformWorld);
    return output;
}
#endif

#ifdef OMNI_SHADOW
[maxvertexcount(18)]
void GSmain(triangle GS_Input input[3],
            inout TriangleStream<PS_Input> triStream)
{
    [unroll]
    for (int i = 0; i < 6; i++)
    {
        PS_Input output;
        output.rtIndex = i;

        [unroll]
        for (int j = 0; j < 3; ++j)
        {
            output.posH = mul(input[j].posW, cb_omniShadowCastViewProj[i]);
            output.posW = input[j].posW.xyz;
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}
#endif

#ifdef CASCADE_SHADOW
[maxvertexcount(3 * CASCADE_SHADOW_SPLIT_LEVEL)]
void GSmain(triangle GS_Input input[3],
            inout TriangleStream<PS_Input> triStream)
{
    [unroll]
    for (int i = 0; i < CASCADE_SHADOW_SPLIT_LEVEL; i++)
    {
        PS_Input output;
        output.rtIndex = i;

        [unroll]
        for (int j = 0; j < 3; ++j)
        {
            output.posH = mul(input[j].posW, cb_cascadeShadowCastViewProj[i]);
            output.posW = input[j].posW.xyz;
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}
#endif

float PSmain(PS_Input input) : SV_DEPTH
{
#ifdef OMNI_SHADOW
    return distance(input.posW, cb_omniShadowLightPosition) / cb_omniShadowLightRange;
#else
    return input.posH.z;
#endif
}
 