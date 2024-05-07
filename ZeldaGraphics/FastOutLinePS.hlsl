#include "Buffers.hlsli"

struct PixelIn
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    uint instance : InstanceID;
};


// Instancing Value 0 : OutLine Color       float4(r, g, b, a)
// Instancing Value 1 : Not Used
// Instancing Value 2 : Draw ID             uint4(id, 0, 0, 0)
// Instancing Value 3 : Not Used

float4 main(PixelIn input) : SV_Target
{
    float4 output = (float4) 0;
    
    // OutLine Test
    uint id = IDMap.Load(uint3(input.uv.x * screenSize.x, input.uv.y * screenSize.y, 0)).r;
    
    const int outlineRange = 3;
    const int filterSize = 2 * outlineRange + 1;
    
    bool highLight = false;
    
    if (id == instancingValue2[input.instance].x)
    {
        [unroll]
        for (int i = 0; i < filterSize; i++)
        {
            [unroll]
            for (int j = 0; j < filterSize; j++)
            {
                int x = i - outlineRange;
                int y = j - outlineRange;
                uint aroundID;
                aroundID = IDMap.Load(uint3(input.uv.x * screenSize.x + x, input.uv.y * screenSize.y + y, 0)).r;
                highLight = highLight | (id != aroundID);
            }
        }
    }
    
    if (highLight)
    {
        output = instancingValue0[input.instance];
    }
    else
    {
        clip(-1.0f);
    }
    
    
    return output;
}