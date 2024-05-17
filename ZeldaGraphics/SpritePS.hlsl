#include "Buffers.hlsli"

struct PixelInputType
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 result = DiffuseMap.Sample(Sampler, input.uv);
    
    return result;
}