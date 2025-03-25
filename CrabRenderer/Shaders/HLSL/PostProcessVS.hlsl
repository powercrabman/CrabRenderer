#include "PostProcessCommon.hlsli"

PS_Input VSmain(VS_2D_INPUT input)
{
    PS_Input output;
    output.position = float4(input.positionL, 0.0f, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}