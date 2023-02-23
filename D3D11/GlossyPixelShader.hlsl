Texture2D MainTexture;
sampler MainSampler;

struct DirectionalLightData
{
    float3 direction;
    float intensity;
    float4 color;
};

struct PointLightData
{
    float3 position;
    float intensity;
    float4 color;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    float maxDist;
};

cbuffer LightData
{
    DirectionalLightData dirLight;
    PointLightData pointLight;
    float4 ambientLight;
};

cbuffer TransparencyData
{
    float transparency;
};

cbuffer CameraData
{
    float3 camPos;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : POSITION;
};

float4 main(PixelInput INPUT) : SV_TARGET
{
    float3 texColor = MainTexture.Sample(MainSampler, INPUT.uv).rgb;
    float3 normal = normalize(INPUT.normal);
    float3 light = ambientLight.rgb;
    float3 viewDir = normalize(camPos - INPUT.worldPos);
    float3 specular;
    
    float lightIntensity = max(dot(normal, -dirLight.direction), 0.0f);
    light += (lightIntensity * dirLight.intensity * dirLight.color.rgb);
    
    float3 halfDir = normalize((-dirLight.direction) + viewDir);
    specular = pow(max(dot(normal, halfDir), 0.0f), dirLight.intensity * 250.0f);
    
    float lightDistance = distance(INPUT.worldPos, pointLight.position);
    float attenuationFactor = 1.0f / (
        pointLight.constantAttenuation +
        pointLight.linearAttenuation * lightDistance +
        pointLight.quadraticAttenuation * lightDistance * lightDistance
    );
    float3 lightDir = normalize(pointLight.position - INPUT.worldPos);
    lightIntensity = max(dot(normal, lightDir), 0.0f);
    lightIntensity *= attenuationFactor;
    lightIntensity = (lightDistance > pointLight.maxDist) ? 0.0f : lightIntensity;
    light += saturate(lightIntensity * pointLight.intensity * pointLight.color.rgb);
    
    halfDir = normalize(lightDir + viewDir);
    specular += pow(max(dot(normal, halfDir), 0.0f), pointLight.intensity * 250.0f) * lightIntensity;
    
    float3 color = texColor * saturate(light);
    color = saturate(color + specular);
    
    return float4(color, transparency);
}