#include "DrawNormalCommon.hlsli"

GS_Input main(VS_Input input)
{
    GS_Input output;
    output.posL = float4(input.posL, 1.f);
    output.normalL = input.normalL;

    return output;
}
