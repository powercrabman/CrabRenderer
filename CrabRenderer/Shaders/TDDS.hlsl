#include "TDCommon.hlsli"

[domain("tri")]
PSInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 uv : SV_DomainLocation,
	const OutputPatch<DSInput, 3> patch)
{
    float4 p = uv.x * patch[0].position 
		 	 + uv.y * patch[1].position 
		 	 + uv.z * patch[2].position;

    PSInput output;
    output.position = mul(p, g_viewProj);
    return output;
}
