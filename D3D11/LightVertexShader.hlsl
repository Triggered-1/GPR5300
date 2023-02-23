cbuffer MatrixBuffer
{
	matrix WorldViewProjectionMatrix;
	matrix WorldMatrix;
};

struct VertexInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

VertexOutput main(VertexInput INPUT)
{
	VertexOutput OUTPUT;
	OUTPUT.position = mul(float4(INPUT.position, 1.0f), WorldViewProjectionMatrix);
	OUTPUT.normal = normalize(mul(INPUT.normal, (float3x3)WorldMatrix));
	OUTPUT.uv = INPUT.uv;

	return OUTPUT;
}