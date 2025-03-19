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

cbuffer ShadowConstant : register(b1)
{
#ifdef BASIC_SHADOW
    MATRIX g_shadowViewProj;
#endif

#ifdef OMNI_SHADOW
    MATRIX g_shadowViewProj[6];
    float3 g_lightPosition;
    float g_fallOffEnd;
#endif

#ifdef CASCADE_SHADOW
    MATRIX g_shadowViewProj[MAX_CASCADE_SHADOW_LEVEL];
#endif
};

#ifdef BASIC_SHADOW
PS_Input VSmain(VS_Input input)
{
    PS_Input output;
    float4 posW = mul(float4(input.posL, 1.f), g_world);
    output.posH = mul(posW, g_shadowViewProj);
    return output;
}
#else
GS_Input VSmain(VS_Input input)
{
    GS_Input output;
    output.posW = mul(float4(input.posL, 1.f), g_world);
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
            output.posH = mul(input[j].posW, g_shadowViewProj[i]);
            output.posW = input[j].posW.xyz;
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}
#endif

#ifdef CASCADE_SHADOW
[maxvertexcount(3 * MAX_CASCADE_SHADOW_LEVEL)]
void GSmain(triangle GS_Input input[3],
            inout TriangleStream<PS_Input> triStream)
{
    [unroll]
    for (int i = 0; i < MAX_CASCADE_SHADOW_LEVEL; i++)
    {
        PS_Input output;
        output.rtIndex = i;

        [unroll]
        for (int j = 0; j < 3; ++j)
        {
            output.posH = mul(input[j].posW, g_shadowViewProj[i]);
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
    return distance(input.posW, g_lightPosition) / g_fallOffEnd;
#else
    return input.posH.z;
#endif
}
 