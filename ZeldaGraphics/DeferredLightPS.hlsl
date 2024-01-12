#include "Buffers.hlsli"

struct PixelIn
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

void CalculateLight(int lightIndex, float3 normal, float3 pos, out float4 diffuse, out float4 ambient, out float4 specular)
{
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    specular = float4(0.f, 0.f, 0.f, 0.f);
    
    float3 viewDirection = normalize(pos);
    
    if (lights[lightIndex].type == LIGHT_TYPE_DIRECTIONAL)
    {
        float3 lightVec = normalize(mul(float4(lights[lightIndex].direction.xyz, 0.f), viewMatrix).xyz);
        ambient = lights[lightIndex].color.ambient;
        
        float diffuseFactor = dot(-lightVec, normal);
        
        if (diffuseFactor > 0.f)
        {
            float3 reflectDir = normalize(lightVec + 2 * (saturate(dot(-lightVec, normal)) * normal));
            float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.f), 16.f);
            
            diffuse += diffuseFactor * lights[lightIndex].color.diffuse;
            specular += specFactor * lights[lightIndex].color.specular;
        }
    }
}

PS_OUT main(PixelIn input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float3 viewPos = Temp0Map.Sample(Sampler, input.uv).xyz;
    if (viewPos.z <= 0.f)
    {
        clip(-1);
    }
    
    float3 viewNormal = Temp1Map.Sample(Sampler, input.uv).xyz;
    
    LightColor color;
    
    CalculateLight(lightIndex, viewNormal, viewPos, color.diffuse, color.ambient, color.specular);
    
    output.diffuse = float4((color.diffuse + color.ambient).xyz, 1.0f);
    output.specular = float4((color.specular).xyz, 1.0f);
    
    return output;
}