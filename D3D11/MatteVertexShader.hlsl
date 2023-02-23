cbuffer MatrixBuffer
{
    float4x4 WVPMatrix;
    float4x4 worldMatrix;
};

struct VertexInput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};
struct VertexOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : POSITION;
};


VertexOutput main(VertexInput INPUT)
{
    VertexOutput OUTPUT;
    OUTPUT.pos = mul(float4(INPUT.pos,1.0f), WVPMatrix);
    OUTPUT.uv = INPUT.uv;
    OUTPUT.normal = normalize(mul(INPUT.normal, (float3x3) worldMatrix));
    OUTPUT.worldPos = mul(float4(INPUT.pos, 1.0f), worldMatrix).xyz;
    
    return OUTPUT;
}