#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

class ZeldaTexture;

class ZeldaModel
{
public:
	~ZeldaModel();

	bool LoadFBX(const std::wstring& filePath);

private:
	ZeldaModel();
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	struct Vertex
	{
		float posX, posY, posZ;
		float r, g, b, a;
		float u, v;
		float norX, norY, norZ;
	};

	struct Material
	{
		Material();

		bool hasTexture;
		std::wstring textureFilePath;

		// 추가될 수 있는 것들
		// diffuseColor 색상
		// specularReflection 반사도
		// Glossiness 광택
		// Transparency 투명도
		// Normal Maps 노말맵
	};

	struct Mesh
	{
		Mesh();
		bool hasPosition;
		bool hasUV;
		bool hasNormal;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Material* material;
	};

	struct Node
	{
		Node();
		~Node();
		Node* parent;
		std::vector<Node*> children;

		std::vector<Mesh*> meshes;
	};

	Node* root;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	void CopyNodeData(Node* node, const aiNode* assimpNode, const aiScene* assimpScene);
	void CopyNodeData_Recursive(Node* node, const aiNode* assimpNode, const aiScene* assimpScene);
	void Clear();

	const static unsigned int DEFAULT_LOAD_FLAG;
};