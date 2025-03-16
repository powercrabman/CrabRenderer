
RWImage2D<float4> g_output : register(u0);
Image2D g_inputTexture : register(t0);
SamplerState g_clampSampler : register(s0);

static const float g_gaussianFactors[7] =
{
    0.0044, 0.0540, 0.2420, 0.3990, 0.2420, 0.0540, 0.0044
};

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint width, height;
    g_inputTexture.GetDimensions(width, height);

    float dx = 1.0 / width;
    float dy = 1.0 / height;

    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < 7; i++)
    {
        float2 pos = float2(DTid.x + 0.5f, DTid.y + 0.5f);
        pos.x = pos.x * dx;
        pos.y = (pos.y + i - 3) * dy;

        color += g_inputTexture.SampleLevel(g_clampSampler, pos, 0) * g_gaussianFactors[i];
    }

    g_output[DTid.xy] = color;
}