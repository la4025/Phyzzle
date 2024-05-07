#include "Buffers.hlsli"
#include "Utility.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 tex : TEXCOORD0;
    uint4 boneIndices : BLENDINDICES;
    float4 weight : BLENDWEIGHT;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
	
	// 적절한 행렬 계산을 위해 위치 벡터를 동차 좌표로 변환한다.
    input.position.w = 1.0f;
    
    float4x4 finalMatrix[4];
    finalMatrix[0] = finalTM[input.boneIndices[0]];
    finalMatrix[1] = finalTM[input.boneIndices[1]];
    finalMatrix[2] = finalTM[input.boneIndices[2]];
    finalMatrix[3] = finalTM[input.boneIndices[3]];
        
    // boneIndices와 weight를 이용해 애니메이션된 position을 계산한다.
    // boneIndices의 값이 unsigned int의 MAX값이면 영향을 받는 본이 없는것으로 생각한다.
    output.position =
        mul(input.boneIndices[0] != 0xffffffffu, mul(mul(input.position, finalMatrix[0]), input.weight[0])) +
        mul(input.boneIndices[1] != 0xffffffffu, mul(mul(input.position, finalMatrix[1]), input.weight[1])) +
        mul(input.boneIndices[2] != 0xffffffffu, mul(mul(input.position, finalMatrix[2]), input.weight[2])) +
        mul(input.boneIndices[3] != 0xffffffffu, mul(mul(input.position, finalMatrix[3]), input.weight[3])) +
        mul(input.boneIndices[0] == 0xffffffffu, input.position);
        
	// 월드, 뷰, 프로젝션 행렬들을 이용해 정점의 위치를 계산한다.
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    
    output.position = mul(output.position, projectionMatrix);
    
    return output;
}