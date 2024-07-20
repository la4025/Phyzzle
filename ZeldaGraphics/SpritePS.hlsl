#include "Buffers.hlsli"

struct PixelInputType
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    uint instance : InstanceID;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor = DiffuseMap.Sample(Sampler, input.uv);
    
    if (useSRGB)
    {
        // SRGB 색 공간에서 선형 색 공간으로
        textureColor = pow(textureColor, 1.0f / 2.2f);
    }
    
    textureColor.r = saturate(textureColor.r * baseColor.r);
    textureColor.g = saturate(textureColor.g * baseColor.g);
    textureColor.b = saturate(textureColor.b * baseColor.b);
    textureColor.a = saturate(textureColor.a * baseColor.a);
    
    return textureColor;
}