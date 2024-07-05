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
    
    float4 shadowMapResult = shadowMap.Sample(Sampler, uv);
    
    //if (shadowMapResult.x == 0.0f &&
    //    shadowMapResult.y == 0.0f &&
    //    shadowMapResult.z == 0.0f &&
    //    shadowMapResult.w == 0.0f)
    //{
    //    return 1.0f;
    //}
    
    float shadowDepth = shadowMapResult.r;
    float result = shadowDepth >= depth;
    
    return result;
}

void CalculateLight(int lightIndex, float3 normal, float3 viewPos, float2 uv, out float4 diffuse, out float4 ambient, out float4 specular, float3 viewPos2, float4 inputPos)
{
    float2 uv2 = float2(inputPos.x / screenSize.x, inputPos.y / screenSize.y);
    
    
    float3 viewPos3 = Temp0Map.Sample(Sampler, uv2).xyz;
    float3 viewPos4 = Temp0Map.Sample(Sampler, uv).xyz;
    
    float3 test = viewPos3;
    float3 test2 = viewPos4;
    
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    specular = float4(0.f, 0.f, 0.f, 0.f);
    
    diffuse = float4(viewPos3, 1); // 임시
    specular = float4(viewPos4, 1); // 임시
    
    float3 viewDirection = normalize(viewPos);
    
    if (lights[lightIndex].type == LIGHT_TYPE_DIRECTIONAL)
    {
        float3 lightVec = normalize(mul(float4(lights[lightIndex].direction.xyz, 0.f), viewMatrix).xyz);
        
        float diffuseFactor = dot(-lightVec, normal);
        
        ambient = lights[lightIndex].color.ambient;
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
        
        
        // Shadow
        if (length(diffuse.xyz) > 0.00001)
        {
            // filterSize x filterSize 크기의 필터 사용
            const int filterSize = 5;
        
            const float deltaX = 1.0f / screenSize.x;
            const float deltaY = 1.0f / screenSize.y;
            float shadowResult[filterSize * filterSize];
            float shadowFactor = 0.0f;
        
            [unroll]
            for (int i = 0; i < filterSize * filterSize; i++)
            {
                int x = (i % filterSize) - (filterSize / 2);
                int y = (i / filterSize) - (filterSize / 2);
            
                float2 offset = float2(deltaX * x, deltaY * y);
            
                float3 currentViewPos = Temp0Map.Sample(Sampler, uv + offset).xyz;
            
                float4 worldPos = mul(float4(currentViewPos, 1.0f), inverse(viewMatrix));
                float4 lightViewPos = mul(worldPos, lightViewMatrix[0]);
                lightViewPos.z -= shadowMapDepthBias; // View좌표계에서 Depth Bias 적용
                float4 shadowClipPos = mul(lightViewPos, lightProjectionMatrix);
            
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, Temp2Map, shadowClipPos, shadowMapSize);
            
                shadowFactor += shadowResult[i];
            }
            shadowFactor /= (float) (filterSize * filterSize);
        
            if (shadowFactor < 0.99f)
            {
                ambient = lights[lightIndex].color.shadow;
            }
            
            diffuse *= shadowFactor;
            specular *= shadowFactor;
        }
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_POINT)
    {
        float ambientRatio = 0.0f;
        float diffuseRatio = 0.0f;
        float specularRatio = 0.0f;
        float distanceRatio = 1.0f;
        
        float3 lightPos = mul(float4(lights[lightIndex].position.xyz, 1.0f), viewMatrix).xyz;
        float3 lightVec = normalize(viewPos - lightPos);
        
        ambientRatio = (dot(-lightVec, normal) < 0.0f) ? (0.0f) : (1.0f);
        diffuseRatio = saturate(dot(-lightVec, normal));
        specularRatio = (dot(-lightVec, normal) < 0.0f) ? (0.0f) : (1.0f);

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
        
        ambient = lights[lightIndex].color.ambient * ambientRatio * distanceRatio;
        diffuse = lights[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
        specular = specFactor * lights[lightIndex].color.specular * specularRatio * distanceRatio;
    }
    else if (lights[lightIndex].type == LIGHT_TYPE_SPOT)
    {
        
    }
    else
    {
        
    }
}

void CalculateShadowPointLight(int lightIndex, float3 normal, float3 viewPos, float2 uv, out float4 diffuse, out float4 ambient, out float4 specular, float3 viewPos2, float4 inputPos)
{
    float2 uv2 = float2(inputPos.x / screenSize.x, inputPos.y / screenSize.y);
    
    
    float3 viewPos3 = Temp0Map.Sample(Sampler, uv2).xyz;
    float3 viewPos4 = Temp0Map.Sample(Sampler, uv).xyz;
    
    float3 test = viewPos3;
    float3 test2 = viewPos4;
    
    diffuse = float4(0.f, 0.f, 0.f, 0.f);
    ambient = float4(0.f, 0.f, 0.f, 0.f);
    specular = float4(0.f, 0.f, 0.f, 0.f);
    
    diffuse = float4(viewPos3, 1); // 임시
    specular = float4(viewPos4, 1); // 임시
    
    float3 viewDirection = normalize(viewPos);
    
    float ambientRatio = 0.0f;
    float diffuseRatio = 0.0f;
    float specularRatio = 0.0f;
    float distanceRatio = 1.0f;
        
    float3 lightPos = mul(float4(lights[lightIndex].position.xyz, 1.0f), viewMatrix).xyz;
    float3 lightVec = normalize(viewPos - lightPos);
        
    ambientRatio = (dot(-lightVec, normal) < 0.0f) ? (0.0f) : (1.0f);
    diffuseRatio = saturate(dot(-lightVec, normal));
    specularRatio = (dot(-lightVec, normal) < 0.0f) ? (0.0f) : (1.0f);

    float d = distance(viewPos, lightPos);
    
    if (lights[lightIndex].range == 0.0f)
    {
        distanceRatio = 0.0f;
    }
    else
    {
        distanceRatio = saturate(1.0f / (lights[lightIndex].atten0 + lights[lightIndex].atten1 * d + lights[lightIndex].atten2 * d * d));
    }

    float3 reflectDir = normalize(lightVec + 2 * (saturate(dot(-lightVec, normal)) * normal));
    float specFactor = pow(max(dot(-viewDirection, reflectDir), 0.f), 16.f);
        
    ambient = lights[lightIndex].color.ambient * ambientRatio * distanceRatio;
    diffuse = lights[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    specular = specFactor * lights[lightIndex].color.specular * specularRatio * distanceRatio;
        
    // Shadow
    if (length(diffuse.xyz) > 0.00001)
    {
        // filterSize x filterSize 크기의 필터 사용
        const int filterSize = 5;
        
        const float deltaX = 1.0f / screenSize.x;
        const float deltaY = 1.0f / screenSize.y;
        float shadowResult[filterSize * filterSize];
        float shadowFactor = 0.0f;
        
        [unroll]
        for (int i = 0; i < filterSize * filterSize; i++)
        {
            int x = (i % filterSize) - (filterSize / 2);
            int y = (i / filterSize) - (filterSize / 2);
            
            float2 offset = float2(deltaX * x, deltaY * y);
            
            float3 currentViewPos = Temp0Map.Sample(Sampler, uv + offset).xyz;
            
            float4 worldPos = mul(float4(currentViewPos, 1.0f), inverse(viewMatrix));
                
                
                
            float3 lightToPixel = worldPos.xyz - lights[lightIndex].position.xyz;

            // 벡터의 절대값
            float3 absLightToPixel = abs(lightToPixel);
            
            // 가장 큰 성분을 찾아 해당 면을 결정
            int faceIndex;
            if (absLightToPixel.x > absLightToPixel.y && absLightToPixel.x > absLightToPixel.z)
            {
                // X 성분이 가장 큰 경우
                faceIndex = (lightToPixel.x > 0.0) ? 0 : 1;
            }
            else if (absLightToPixel.y > absLightToPixel.x && absLightToPixel.y > absLightToPixel.z)
            {
                // Y 성분이 가장 큰 경우
                faceIndex = (lightToPixel.y > 0.0) ? 2 : 3;
            }
            else
            {
                // Z 성분이 가장 큰 경우
                faceIndex = (lightToPixel.z > 0.0) ? 4 : 5;
            }
                
            float4 lightViewPos = mul(worldPos, lightViewMatrix[faceIndex]);
            float4 shadowClipPos = mul(lightViewPos, lightProjectionMatrix);
            shadowClipPos.xyz = shadowClipPos.xyz / shadowClipPos.w;
            shadowClipPos.z = saturate(shadowClipPos.z - pointLightDepthBias); // NDC좌표계에서 Depth Bias 적용
                
            if (faceIndex == 0)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, Temp2Map, shadowClipPos, shadowMapSize);
            }
            else if (faceIndex == 1)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, DiffuseMap, shadowClipPos, shadowMapSize);
            }
            else if (faceIndex == 2)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, NormalMap, shadowClipPos, shadowMapSize);
            }
            else if (faceIndex == 3)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, ARMMap, shadowClipPos, shadowMapSize);
            }
            else if (faceIndex == 4)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, HeightMap, shadowClipPos, shadowMapSize);
            }
            else if (faceIndex == 5)
            {
                shadowResult[i] = CalculateShadowFactor(ShadowSampler, Temp0Map, EmissionMap, shadowClipPos, shadowMapSize);
            }
            
            shadowFactor += shadowResult[i];
        }
            
        shadowFactor /= (float) (filterSize * filterSize);
        
        diffuse *= shadowFactor;
        specular *= shadowFactor;
    }
}

#endif