#include "../../inc/ShaderSharedData.h"

struct PS_Input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

SamplerState SamplerLinearClamp : register(s0);
