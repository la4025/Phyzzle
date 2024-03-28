#ifndef _LIGHT_HLSLI_
#define _LIGHT_HLSLI_

#include "Buffers.hlsli"
#include "Matrix.hlsli"

float Median(float d0, float d1, float d2)
{
    if ((d0 >= d1 && d0 <= d2) || (d0 <= d1 && d0 >= d2))
    {
        // d0이 중앙값인 경우
        return d0;
    }
    else if ((d1 >= d0 && d1 <= d2) || (d1 <= d0 && d1 >= d2))
    {
        // d1이 중앙값인 경우
        return d1;
    }
    else
    {
        // d2가 중앙값인 경우
        return d2;
    }
}

float MedianFilter(
    float d0, float d1, float d2,
    float d3, float d4, float d5,
    float d6, float d7, float d8)
{
    return Median(Median(d0, d1, d2), Median(d3, d4, d5), Median(d6, d7, d8));
}

float CalculateShadowFactor(SamplerComparisonState shadowsampler, Texture2D positionMap, Texture2D shadowMap, float4 shadowPos, float mapSize)
{
    float depth = shadowPos.z;
    
    const float delta = 1.0f / mapSize;
    
    const float2 offsets[9] =
    {
        float2(-delta, -delta), float2(0.0f, -delta), float2(delta, -delta),
		float2(-delta, 0.0f), float2(0.0f, 0.0f), float2(delta, 0.0f),
		float2(-delta, +delta), float2(0.0f, +delta), float2(delta, +delta)
    };
    
    float2 uv = shadowPos.xy;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    if (uv.x < 0)
        return 1.0f;
    if (uv.x > 1)
        return 1.0f;
    if (uv.y < 0)
        return 1.0f;
    if (uv.y > 1)
        return 1.0f;
    
    float result[9];
    [unroll]
    for (int i = 0; i < 9; i++)
    {
        float shadowDepth = shadowMap.Sample(Sampler, uv + offsets[i]).r;
        result[i] = shadowDepth >= depth;
    }
    
    float interpolatedResult =
        /*
        MedianFilter(
            result[0], result[1], result[2],
            result[3], result[4], result[5],
            result[6], result[7], result[8]
        );
        //*/
        
        //*
        (
            result[0] + result[1] + result[2] +
            result[3] + result[4] + result[5] +
            result[6] + result[7] + result[8]
        ) / 9.0f;
        //*/
    
        ///*
        //result[5];
        //*/
    
    return interpolatedResult;
    
    //return 1.0f;
}

void CalculateLight(int lightIndex, float3 normal, float3 viewPos, out float4 diffuse, out float4 ambient, out float4 specular)
{
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    specular = float4(0.f, 0.f, 0.f, 0.f);
    
    float3 viewDirection = normalize(viewPos);
    
    if (lights[lightIndex].type == LIGHT_TYPE_DIRECTIONAL)
    {
        float3 lightVec = normalize(mul(float4(lights[lightIndex].direction.xyz, 0.f), viewMatrix).xyz);
        ambient = lights[lightIndex].color.ambient;
        
        float diffuseFactor = dot(-lightVec, normal);
        
        // Original Phong 반사벡터 사용
        float3 reflectDir = normalize(lightVec + 2.0f * (dot(-lightVec, normal) * normal));
        float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.0f), 16.0f);
        diffuse = max(0.0f, diffuseFactor) * lights[lightIndex].color.diffuse;
        specular = specFactor * lights[lightIndex].color.specular;
        
        // Blinn-Phong 하프벡터 사용
        //float3 halfVec = normalize(-lightVec - viewDirection);
        //float NdotH = max(0, dot(normal, halfVec));
        //float specFactor = pow(NdotH, 16.0f);
        //diffuse = diffuseFactor * lights[lightIndex].color.diffuse;
        //specular = specFactor * lights[lightIndex].color.specular;
        
        
        float4 worldPos = mul(float4(viewPos, 1.0f), inverse(viewMatrix));
        float4 lightViewPos = mul(worldPos, lightViewMatrix);
        lightViewPos.z -= shadowMapDepthBias; // View좌표계에서 Depth Bias 적용
        float4 shadowClipPos = mul(lightViewPos, lightProjectionMatrix);
        
        float shadow = CalculateShadowFactor(ShadowSampler, Temp0Map, Temp2Map, shadowClipPos, shadowMapSize);
        
        diffuse *= shadow;
        specular *= shadow;
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_POINT)
    {
        float diffuseRatio = 0.0f;
        float distanceRatio = 1.0f;
        
        float3 lightPos = mul(float4(lights[lightIndex].position.xyz, 1.0f), viewMatrix).xyz;
        float3 lightVec = normalize(viewPos - lightPos);
        
        diffuseRatio = saturate(dot(-lightVec, normal));

        float d = distance(viewPos, lightPos);
        if (lights[lightIndex].range == 0.0f)
        {
            distanceRatio = 0.0f;
        }
        else
        {
            distanceRatio = saturate(1.0f - pow(d / lights[lightIndex].range, 2.0f));
        }

        float3 reflectDir = normalize(lightVec + 2 * (saturate(dot(-lightVec, normal)) * normal));
        float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.f), 16.f);
        
        ambient = lights[lightIndex].color.ambient * distanceRatio;
        diffuse = lights[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
        specular = specFactor * lights[lightIndex].color.specular * distanceRatio;
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_SPOT)
    {
        
    }
    else
    {
        
    }
}

#endif