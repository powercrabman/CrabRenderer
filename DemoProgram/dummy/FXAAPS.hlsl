#include "PostProcessingCommon.hlsli"

static const float FXAA_SPAN_MAX = 8.0;
static const float FXAA_REDUCE_MUL = 1.0 / 8.0;
static const float FXAA_REDUCE_MIN = 1.0 / 128.0;

float3 SampleColor(float2 uv)
{
    return g_texture.Sample(g_sampler, uv).rgb;
}

float3 FxaaAA(float2 p)
{
    float3 rgbNW = SampleColor(p + float2(-1.0, -1.0) * float2(g_dx, g_dy));
    float3 rgbNE = SampleColor(p + float2(1.0, -1.0) * float2(g_dx, g_dy));
    float3 rgbSW = SampleColor(p + float2(-1.0, 1.0) * float2(g_dx, g_dy));
    float3 rgbSE = SampleColor(p + float2(1.0, 1.0) * float2(g_dx, g_dy));
    float3 rgbM = SampleColor(p);

    float3 lumaVec = float3(0.299, 0.587, 0.114);

    float lumaNW = dot(rgbNW, lumaVec);
    float lumaNE = dot(rgbNE, lumaVec);
    float lumaSW = dot(rgbSW, lumaVec);
    float lumaSE = dot(rgbSE, lumaVec);
    float lumaM = dot(rgbM, lumaVec);

    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float lumaSum = lumaNW + lumaNE + lumaSW + lumaSE;
    float dirReduce = max(lumaSum * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = clamp(dir * rcpDirMin,
                float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX)) * float2(g_dx, g_dy);

    float3 rgbA = 0.5 * (
        SampleColor(p + dir * (1.0 / 3.0 - 0.5)) +
        SampleColor(p + dir * (2.0 / 3.0 - 0.5))
    );

    float3 rgbB = rgbA * 0.5 + 0.25 * (
        SampleColor(p + dir * (0.0 / 3.0 - 0.5)) +
        SampleColor(p + dir * (3.0 / 3.0 - 0.5))
    );

    float lumaB = dot(rgbB, lumaVec);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    return ((lumaB < lumaMin) || (lumaB > lumaMax)) ? rgbA : rgbB;
}

float4 main(PSInput input) : SV_Target
{
    float3 aaColor = FxaaAA(input.texCoord);
    return float4(aaColor, 1.0);
}
