#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <string>
#include <queue>

#include "ZeldaGraphicsDefine.h"
#include "FBXData.h"

#include "ConstantBuffer.h"

class ZeldaMesh;
class ZeldaMaterial;
class ZeldaShader;
class ZeldaLight;

class ZeldaModel
{
private:
	struct Node
	{
		Node* parent;
		std::vector<Node*> children;

		std::wstring name;

		std::vector<unsigned int> meshes;

		DirectX::XMMATRIX finalTM;
		DirectX::XMMATRIX transformMatrix;
		DirectX::XMMATRIX offsetMatrix;
	};

	struct Mesh
	{
		ZeldaMesh* meshData;

		unsigned int materialIndex;
	};

	struct Animation
	{
		std::wstring name;

		double duration; // Æ½´ç ½Ã°£
		double tickPerSecond; // ½Ã°£´ç Æ½

		// key: boneIndex
		// value: map<time, Matrix> 
		std::map<unsigned int, std::map<double, DirectX::XMMATRIX>> animationKey;
	};

public:
	~ZeldaModel();

	void Render(
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
		ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
		ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer,
		ConstantBuffer<UseBufferType, ShaderType::PixelShader>* useConstBuffer,
		ConstantBuffer<ColorBufferType, ShaderType::PixelShader>* colorConstBuffer,
		DirectX::XMMATRIX worldMatrix,
		ZeldaShader* shader,
		ZeldaLight* light
	);

private:
	ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel);
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	void CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable);

	Node* root;
	std::vector<Node*> bones;
	std::vector<ZeldaMesh*> meshes;
	std::vector<unsigned int> materialIndex; // meshes[0]Àº materials[materialIndex[0]]À» °¡Áü
	std::vector<ZeldaMaterial*> materials;
	std::vector<Animation*> animationList;

	friend class ResourceManager;
};