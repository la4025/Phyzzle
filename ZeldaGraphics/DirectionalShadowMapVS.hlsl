#include "Buffers.hlsli"
#include "Utility.hlsli"

struct VertexIn
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float4 clipPos : POSITIONT;
};

VertexOut main(VertexIn input)
{
    VertexOut output = (VertexOut) 0;
    
    // 적절한 행렬 계산을 위해 위치 벡터를 동차 좌표로 변환한다.
    input.position.w = 1.0f;
    
    float4x4 finalMatrix[4];
    finalMatrix[0] = GetFinalTM(input.boneIndices[0], animationInfo.time);
    finalMatrix[1] = GetFinalTM(input.boneIndices[1], animationInfo.time);
    finalMatrix[2] = GetFinalTM(input.boneIndices[2], animationInfo.time);
    finalMatrix[3] = GetFinalTM(input.boneIndices[3], animationInfo.time);
        
    // boneIndices와 weight를 이용해 애니메이션된 position을 계산한다.
    // boneIndices의 값이 unsigned int의 MAX값이면 영향을 받는 본이 없는것으로 생각한다.
    output.pos =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
    
    output.pos = mul(mul(mul(output.pos, worldMatrix), lightViewMatrix), lightProjectionMatrix);
    output.clipPos = output.pos;
    
    return output;
}