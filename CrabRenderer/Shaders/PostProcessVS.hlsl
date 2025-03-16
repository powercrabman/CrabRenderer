#include "PostProcessCommon.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;
    output.position = float4(input.position, 0.0f, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}