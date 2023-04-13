cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	float4 g_DiffuseColor;
};

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	return g_DiffuseColor;
}