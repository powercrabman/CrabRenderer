#include "DepthVisualizeCommon.hlsli"

PS_Input main(VS_Input input)
{
    PS_Input output;
    output.posH = float4(input.posL, 0.0f, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}