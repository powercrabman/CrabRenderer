#include "DrawFireballCommon.hlsli"

GSInput main(VSInput pos)
{
    GSInput output;
    output.pos = float4(pos.pos, 1.0f);
    return output;
}