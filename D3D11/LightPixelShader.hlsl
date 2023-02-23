Texture2D MainTexture;
SamplerState MainSampler;

cbuffer	LightBuffer // is 16 byte aligned
{
	float3 LightDirection;
	float LightIntensity;
	float4 LightColor;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

float4 main(PixelInput INPUT) : SV_TARGET
{
	float4 textureColor = MainTexture.Sample(MainSampler, INPUT.uv);

	// light data
	float3 normal = normalize(INPUT.normal);
	float3 light = normalize(-LightDirection);

	//diffuse
	float4 diffuseColor = 0;
	float diffuse = max(dot(normal, light), 0);
	diffuseColor = LightColor * (diffuse * LightIntensity);

	// Texture * (Ambient + Diffuse) + Specular + Emission
	return textureColor * diffuseColor;
}