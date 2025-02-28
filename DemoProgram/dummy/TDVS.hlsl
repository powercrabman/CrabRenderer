#include "TDCommon.hlsli"

HSInput main(VSInput input)
{
    HSInput output;
    output.position = mul(float4(input.position, 1.0f), g_world);
    return output;
}