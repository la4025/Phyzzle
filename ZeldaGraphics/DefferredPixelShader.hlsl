Texture2D Texture : register(t0);
Texture2D NormalMap : register(t1);

SamplerState Sampler : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 lightDirection;
    float padding;
};

cbuffer UseBufferType : register(b1)
{
    unsigned int useNormal;
    unsigned int useTexture;
    unsigned int useColor;
    unsigned int useSRGB;

    unsigned int useTemp0;
    unsigned int useTemp1;
    unsigned int useTemp2;
    unsigned int useTemp3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct PixelOutputType
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
    float4 depth : SV_Target3;
};
    
cbuffer ColorBufferType : register(b2)
{
    float4 baseColor;
};

PixelOutputType main(PixelInputType input)
{
    PixelOutputType output = (PixelOutputType) 0;
    
    float4 textureColor;
    float3 lightDir;
    float diffuseIntensity;
    float specularIntensity = 0.0f;
    float4 color;
    
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    if (useTexture == true)
    {
        textureColor = Texture.Sample(Sampler, input.tex);
        
        if (useSRGB)
        {
            // SRGB 색 공간에서 선형 색 공간으로.
            textureColor = pow(textureColor, 1.0f / 2.2f);
        }
    }
    else if (useColor == true)
    {
        textureColor = baseColor;
    }
    else
    {
        textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    // Invert the light direction for calculations.
    lightDir = -lightDirection;
	// Calculate the amount of light on this pixel.
    diffuseIntensity = saturate(dot(input.normal, lightDir));
    
    if (diffuseIntensity > 0.0f)
    {
        float3 viewDir = float3(0.0f, 0.0f, -1.0f);
        float specFactor = pow(saturate(dot(reflect(-viewDir, input.normal), lightDir)), 32.0f);
        specularIntensity = specFactor;
    }

    // Calculate final color using the Phong lighting model.
    output.color = (ambient + diffuseIntensity * diffuse) * textureColor + specularIntensity * specular;
    
    output.position = input.position;
    output.normal = float4(input.normal, 1.0f);
    
    return output;
}