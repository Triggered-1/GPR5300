Texture2D MainTexture;
SamplerState MainSampler;

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(PixelInput INPUT) : SV_TARGET
{
	return MainTexture.Sample(MainSampler, INPUT.uv); // texture color
//return 1.0f - MainTexture.Sample(MainSampler, INPUT.uv); // inverted texture color
}