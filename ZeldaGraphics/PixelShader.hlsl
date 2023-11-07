Texture2D shaderTexture : register(t0);
SamplerState SampleType;

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
    unsigned int useTemp0;
};
    
cbuffer ColorBufferType : register(b2)
{
    float4 baseColor;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float diffuseIntensity;
    float specularIntensity = 0.0f;
    float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    if (useTexture == true)
    {
        textureColor = shaderTexture.Sample(SampleType, input.tex);
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
	/* 아래 코드에서 노멀벡터와 빛벡터의 내적을 구하고 [0, 1] 구간으로 clamp하는건데
	만약 노멀벡터와 빛벡터 사이의 각이 90도보다 크다면 내적값은 음수가 되므로
	clamp한 결과는 0이 됨(90도이어도 0). 따라서 lightIntensity = 0이고, 빛을 아예 안받는게 된다.
	각이 90도보다 작다면 clamp한 결과는 0보다 크게 된다. 
	각이 작으면 작을 수록 clamp한 결과는 1에 가까워지고, 빛의 세기는 강해진다. */
    
    diffuseIntensity = saturate(dot(input.normal, lightDir));
    
    if (diffuseIntensity > 0.0f)
    {
        float3 viewDir = float3(0.0f, 0.0f, -1.0f);
        float specFactor = pow(saturate(dot(reflect(-viewDir, input.normal), lightDir)), 32.0f);
        specularIntensity = specFactor;
    }

    // Calculate final color using the Phong lighting model.
    color = (ambient + diffuseIntensity * diffuse) * textureColor + specularIntensity * specular;

    return color;
}