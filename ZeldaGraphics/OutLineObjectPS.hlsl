#include "Buffers.hlsli"

struct PixelInputType
{
    float4 position : SV_POSITION;
};

uint main(PixelInputType input) : SV_Target0
{
    uint output = (uint) 0;
    
    output = objectID;
    
    return output;
}