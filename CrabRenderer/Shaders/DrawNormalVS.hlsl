#include "DrawNormalCommon.hlsli"

GSInput main(VSInput input)
{
	GSInput output;
	output.normalL = input.normalL;
	output.posL = float4(input.posL, 1.f);
	
	return output;
}