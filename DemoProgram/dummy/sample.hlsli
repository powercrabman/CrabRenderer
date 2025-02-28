float4 VSmain(float2 in_pos : POSITION) : SV_POSITION
{
    return float4(in_pos, 0.f, 1.f);
}

float4 PSmain(float4 in_pos : SV_POSITION) : SV_TARGET
{
    return float4(1.f, 0.f, 0.f, 1.f);
}