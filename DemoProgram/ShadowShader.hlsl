#include "Shaders/ShaderCommon.hlsli"

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

struct PSOutput
{
    float4 outColor : SV_TARGET;

#ifdef OMNI_SHADOW
    float outDepth : SV_DEPTH;
#endif

};


cbuffer ShadowConstant : register(b1)
{
#ifdef BASIC_SHADOW
    MATRIX g_shadowViewProj;
#endif

#ifdef OMNI_SHADOW
    MATRIX g_shadowViewProj[6];
#endif

#ifdef CASCADE_SHADOW
    MATRIX g_shadowViewProj[4];
#endif

    float3 lightPosition;
    float fallOffEnd;
};

#ifndef BASIC_SHADOW
GS_Input VSmain(VS_Input input)
{
    GS_Input output;
    output.posW = mul(float4(input.normalL, 1.f), g_world);
    return output;
}
#else
PS_Input VSmain(VS_Input input)
{
    PS_Input output;
    float4 posW = mul(float4(input.posL, 1.f), g_world);
    output.posH = mul(posW, g_viewProj);
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
[maxvertexcount(12)]
void GSmain(triangle GS_Input input[3],
            inout TriangleStream<PS_Input> triStream)
{
    [unroll]
    for (int i = 0; i < 4; i++)
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

PSOutput PSmain(PS_Input input)
{
    PSOutput output;
    output.outColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

#ifdef OMNI_SHADOW
    output.outDepth = distance(input.posW, lightPosition) / fallOffEnd;
#endif

    return output;
}
 