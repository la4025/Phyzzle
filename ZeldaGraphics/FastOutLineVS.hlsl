#include "Buffers.hlsli"

struct VertexIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
    uint instance : SV_InstanceID;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    uint instance : InstanceID;
};

// Instancing Value 0 : Not Used
// Instancing Value 1 : Not Used
// Instancing Value 2 : Draw ID             uint4(id, 0, 0, 0)
// Instancing Value 3 : Not Used

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    
    output.pos = float4((input.position * 2.0f).xyz, 1.0f);
    output.uv = input.tex;
    output.instance = input.instance;
    
    return output;
}