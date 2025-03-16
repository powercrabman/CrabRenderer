#include "BillboradDemoCommon.hlsli"

GSInput main(VSInput input)
{
	GSInput output;
	output.pos = float4(input.pos, 1.f);
	return output;
}