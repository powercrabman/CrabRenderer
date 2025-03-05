#define MATRIX row_major float4x4 

// for debug
//#define BASIC_SHADOW
//#define OMNI_SHADOW
//#define CASCADE_SHADOW

struct VSInput
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangentL : TANGENT;
};

struct GSInput
{
    float4 posW : SV_POSITION;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    uint rtIndex : SV_RenderTargetArrayIndex;
};

cbuffer TransformConstant : register(b0)
{
    MATRIX g_world;
    MATRIX g_worldInvTranspose;
};

cbuffer ShadowConstant : register(b1)
{
#ifdef OMNI_SHADOW
    MATRIX g_shadowViewProj[6];
#endif

#ifdef BASIC_SHADOW
    MATRIX g_shadowViewProj;
#endif

#ifdef CASCADE_SHADOW
    MATRIX g_shadowViewProj[4];
#endif

};

GSInput VSmain(VSInput input)
{
    GSInput output;
    output.posW = mul(float4(input.posL, 1.0f), g_world);

#ifdef BASIC_SHADOW
    output.posW = mul(output.posW, g_shadowViewProj);
#endif

    return output;
}

#ifdef OMNI_SHADOW
[maxvertexcount(18)]
void GSmain(triangle GSInput input[3],
            inout TriangleStream<PSInput> triStream)
{
    [unroll]
    for (int i = 0; i < 6; i++)
    {
        PSInput output;
        output.rtIndex = i;

        [unroll]
        for (int j = 0; j < 3; ++j)
        {;
            output.posH = mul(input[j].posW, g_shadowViewProj[i]);
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}
#endif

#ifdef CASCADE_SHADOW
[maxvertexcount(12)]
void GSmain(triangle GSInput input[3],
            inout TriangleStream<PSInput> triStream)
{
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        [unroll]
        for (int j = 0; j < 3; ++j)
        {
            PSInput output;
            output.posH = mul(input[j].posW, g_shadowViewProj[i]);
            output.rtIndex = i;
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}
#endif

float4 PSmain(PSInput input) : SV_Target
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}
 