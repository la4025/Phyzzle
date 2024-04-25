#include "Buffers.hlsli"

struct PixelIn
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PixelIn input) : SV_Target
{
    float4 output = (float4) 0;
    
    float4 diffuseLight = Temp1Map.Sample(Sampler, input.uv);
    
    if (diffuseLight.x == 0.0f && diffuseLight.y == 0.0f, diffuseLight.z == 0.0f)
    {
        clip(-1);
    }
    
    float4 color = Temp0Map.Sample(Sampler, input.uv);
    float4 specular = Temp2Map.Sample(Sampler, input.uv);
    
    output = (color * diffuseLight) + specular;
    
    // OutLine Test
    //uint id = IDMap.Load(uint3(input.uv.x * 1920, input.uv.y * 1080, 0)).r;
    
    //const int outlineRange = 2;
    //const int filterSize = 2 * outlineRange + 1;
    
    //bool highLight = false;
    
    //if (id == 8)
    //{
    //    [unroll]
    //    for (int i = 0; i < filterSize; i++)
    //    {
    //        [unroll]
    //        for (int j = 0; j < filterSize; j++)
    //        {
    //            int x = i - outlineRange;
    //            int y = j - outlineRange;
    //            uint aroundID;
    //            aroundID = IDMap.Load(uint3(input.uv.x * 1920 + x, input.uv.y * 1080 + y, 0)).r;
    //            highLight = highLight | (id != aroundID);
    //        }
    //    }
    //}
    
    //if (highLight)
    //{
    //    output = float4(1.0f, 1.0f, 0.0f, 1.0f);
    //}
    
    return output;
}