struct VS_INPUT
{
    float2 posL : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.posH = float4(input.posL, 0.0f, 1.0f);
    output.uv = input.uv;
    return output;
}
