#pragma once

#include <vector>
#include <string>

namespace ModelingData
{
    struct ZNode
    {
        ZNode* parent;
        std::vector<ZNode*> children;
        std::vector<ZMesh*> meshes;
    };

    struct ZVertex
    {
        float x, y, z; // 정점의 위치 좌표
        float nx, ny, nz; // 정점의 법선 벡터 좌표
        float u, v; // 텍스처 좌표
        float r, g, b, a; // 색
    };

    struct ZMesh
    {
        bool hasPosition;

        std::vector<ZVertex> vertices; // 정점 데이터
        std::vector<unsigned int> indices; // 정점 인덱스 데이터
    };

    struct ZMaterial
    {
        // 머테리얼 정보: 앰비언트, 디퓨즈, 스펙큘러 색상, 텍스처 경로 등을 포함할 수 있습니다.
        float ambient[4]; // 앰비언트 색상 (R, G, B, A)
        float diffuse[4]; // 디퓨즈 색상 (R, G, B, A)
        float specular[4]; // 스펙큘러 색상 (R, G, B, A)
        float shininess; // 스펙큘러 지수
        std::wstring texturePath; // 머테리얼에 사용될 텍스처 파일 경로
    };

    struct ZModel
    {
        ZNode* rootNode;
        std::vector<ZMesh> meshes; // 모델의 메시 데이터
        std::vector<ZMaterial> materials;
    };


}