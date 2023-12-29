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

PixelOutputType main(PixelInputType input)
{
    PixelOutputType output = (PixelOutputType) 0;
    
    
    
    return output;
}