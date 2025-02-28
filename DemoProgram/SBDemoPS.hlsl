struct PS_Input
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_Input input) : SV_TARGET
{
    return input.color;
}