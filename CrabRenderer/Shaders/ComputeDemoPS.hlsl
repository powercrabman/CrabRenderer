struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D g_inputTexture : register(t0);
SamplerState g_wrapSampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    return g_inputTexture.Sample(g_wrapSampler, input.uv);
}