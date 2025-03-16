#include "TDCommon.hlsli"

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<HSInput, 3> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT output;
	
	float dist = length(g_eyePos - ip[0].position.xyz);
	
	float factor = floor(-6.3f * dist + 64);
	factor = clamp(factor, 1, 64);
	
	output.EdgeTessFactor[0] = factor;
	output.EdgeTessFactor[1] = factor;
    output.EdgeTessFactor[2] = factor;

    output.InsideTessFactor = factor;
	
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
DSInput main(
	InputPatch<HSInput, 3> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
	DSInput output;

	output.position = ip[i].position;

	return output;
}
