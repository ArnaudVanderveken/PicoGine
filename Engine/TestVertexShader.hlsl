struct VS_Input
{
	float2 Position: POSITION;
	float4 Color: COLOR0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 Color: COLOR0;
};

VS_Output main(VS_Input input)
{
	VS_Output output = (VS_Output)0;

	output.Position = float4(input.Position.x, input.Position.y, 0.0f, 1.0f);
	output.Color = input.Color;

	return output;
}