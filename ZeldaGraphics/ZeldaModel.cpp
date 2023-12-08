#include "ZeldaModel.h"

#include "MathConverter.h"
#include "ZeldaMesh.h"
#include "ZeldaMaterial.h"
#include "ZeldaCamera.h"
#include "GraphicsResourceID.h"
#include "ZeldaShader.h"
#include "ZeldaLight.h"
#include "ZeldaMaterial.h"
#include "ZeldaTexture.h"
#include "ResourceManager.h"
#include "ConstantBufferManager.h"

using namespace DirectX;

ZeldaModel::~ZeldaModel()
{
	// mesh
	

	// material




	// 채워야함
}

void ZeldaModel::Render(
	ID3D11DeviceContext* deviceContext,
	ConstantBuffer<MatrixBufferType, ShaderType::VertexShader>* matrixConstBuffer,
	ConstantBuffer<BoneBufferType, ShaderType::VertexShader>* boneConstBuffer,
	ConstantBuffer<LightBufferType, ShaderType::PixelShader>* lightConstBuffer,
	ConstantBuffer<UseBufferType, ShaderType::PixelShader>* useConstBuffer,
	ConstantBuffer<ColorBufferType, ShaderType::PixelShader>* colorConstBuffer,
	DirectX::XMMATRIX worldMatrix,
	ZeldaShader* shader,
	ZeldaLight* light)
{
	// 애니메이션 시간에 따라 Bone정보 변경
	std::queue<std::pair<Node*, DirectX::XMMATRIX>> nodeQueue;

	nodeQueue.push({ root, root->transformMatrix });

	while (!nodeQueue.empty())
	{
		auto current = nodeQueue.front();
		nodeQueue.pop();
		Node* currentNode = current.first;
		DirectX::XMMATRIX currentMatrix = current.second;

		currentNode->finalTM = currentNode->offsetMatrix;// * currentMatrix;

		for (int i = 0; i < currentNode->children.size(); i++)
		{
			Node* nextNode = currentNode->children[i];
			DirectX::XMMATRIX nextMatrix = currentMatrix * nextNode->transformMatrix;

			nodeQueue.push({ nextNode, nextMatrix });
		}
	}

	BoneBufferType boneBuffer;
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i] == nullptr) continue;
		boneBuffer.boneTM[i] = bones[i]->finalTM;
	}

	ZeldaCamera* currentcamera = ResourceManager::GetInstance().GetCamera(ZeldaCamera::GetMainCamera());

	// 셰이더에 넘기는 행렬을 전치를 한 후 넘겨야 한다.
	matrixConstBuffer->SetData({ XMMatrixTranspose(worldMatrix), XMMatrixTranspose(currentcamera->GetViewMatrix()), XMMatrixTranspose(currentcamera->GetProjMatrix()) });
	boneConstBuffer->SetData(boneBuffer);
	lightConstBuffer->SetData({ light->GetAmbient(), light->GetDiffuseColor(), light->GetSpecular(), light->GetDirection() });
	colorConstBuffer->SetData({ { 1, 1, 1, 1 } });

	// 모든 메쉬 그리기
	for (int i = 0; i < meshes.size(); i++)
	{
		ZeldaMesh* currentMesh = meshes[i];
		currentMesh->Render(deviceContext);
		int indexCount = currentMesh->GetIndexCount();

		useConstBuffer->SetData({ false, (materials[materialIndex[i]]->useDiffuseMap ), true, (materials[materialIndex[i]]->diffuseMap->UseSRGB() == false) && materials[materialIndex[i]]->diffuseMap->UseSRGB() });

		ConstantBufferManager::GetInstance().SetBuffer();

		shader->Render(deviceContext, indexCount, materials[materialIndex[i]]->diffuseMap);
	}
}

ZeldaModel::ZeldaModel(ID3D11Device* device, FBXLoader::Model* fbxModel) :
	root(nullptr)
{
	root = new Node();

	std::map<std::wstring, Node*> nodeTable;
	CopyNode(root, fbxModel->root, nodeTable);

	bones.assign(256, nullptr);
	for (int i = 0; i < fbxModel->boneList.size(); i++)
	{
		if (fbxModel->boneList[i] != nullptr)
		{
			bones[i] = nodeTable[fbxModel->boneList[i]->name];
		}
	}

	for (int i = 0; i < fbxModel->meshList.size(); i++)
	{
		FBXLoader::Mesh* fbxMesh = fbxModel->meshList[i];

		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;

		for (int j = 0; j < fbxMesh->vertices.size(); j++)
		{
			VertexType vertexType;
			vertexType.position.x = fbxMesh->vertices[j].position.x;
			vertexType.position.y = fbxMesh->vertices[j].position.y;
			vertexType.position.z = fbxMesh->vertices[j].position.z;

			vertexType.normal.x = fbxMesh->vertices[j].normal.x;
			vertexType.normal.y = fbxMesh->vertices[j].normal.y;
			vertexType.normal.z = fbxMesh->vertices[j].normal.z;

			vertexType.texture.x = fbxMesh->vertices[j].textureCoordinate.u;
			vertexType.texture.y = fbxMesh->vertices[j].textureCoordinate.v;

			vertexType.boneIndices.x = 0;
			vertexType.boneIndices.y = 0;
			vertexType.boneIndices.z = 0;
			vertexType.boneIndices.w = 0;

			if (fbxMesh->vertices[j].boneIndices[0] < 256) vertexType.boneIndices.x = fbxMesh->vertices[j].boneIndices[0];
			if (fbxMesh->vertices[j].boneIndices[1] < 256) vertexType.boneIndices.y = fbxMesh->vertices[j].boneIndices[1];
			if (fbxMesh->vertices[j].boneIndices[2] < 256) vertexType.boneIndices.z = fbxMesh->vertices[j].boneIndices[2];
			if (fbxMesh->vertices[j].boneIndices[3] < 256) vertexType.boneIndices.w = fbxMesh->vertices[j].boneIndices[3];

			vertexType.weight.x = fbxMesh->vertices[j].weight[0];
			vertexType.weight.y = fbxMesh->vertices[j].weight[1];
			vertexType.weight.z = fbxMesh->vertices[j].weight[2];
			vertexType.weight.w = fbxMesh->vertices[j].weight[3];

			vertices.push_back(vertexType);
		}

		for (int j = 0; j < fbxMesh->indices.size(); j++)
		{
			indices.push_back(fbxMesh->indices[j]);
		}

		meshes.push_back(new ZeldaMesh(device, vertices, indices));
		materialIndex.push_back(fbxMesh->materialIndex);
	}

	for (int i = 0; i < fbxModel->materialList.size(); i++)
	{
		FBXLoader::Material* fbxMaterial = fbxModel->materialList[i];
		ZeldaMaterial* material = new ZeldaMaterial(device, DirectX::XMFLOAT4(fbxMaterial->baseColor.r, fbxMaterial->baseColor.g, fbxMaterial->baseColor.b, fbxMaterial->baseColor.a), fbxMaterial->diffuseMap);
		materials.push_back(material);
	}

	for (int i = 0; i < fbxModel->animationList.size(); i++)
	{
		FBXLoader::Animation* fbxAnimation = fbxModel->animationList[i];

		Animation* animation = new Animation();
		animation->name = fbxAnimation->name;
		animation->duration = fbxAnimation->duration;
		animation->tickPerSecond = fbxAnimation->tickPerSecond;
		
		for (auto iter = fbxAnimation->animationKey.begin(); iter != fbxAnimation->animationKey.end(); iter++)
		{
			unsigned int boneIndex = iter->first;

			std::map<double, DirectX::XMMATRIX> keys;

			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
			{
				double time = iter2->first;
				DirectX::XMMATRIX matrix = MathConverter::EigenMatrixToXMMatrix(iter2->second);
				keys[time] = matrix;
			}
			animation->animationKey[boneIndex] = keys;
		}

		animationList.push_back(animation);
	}
}

void ZeldaModel::CopyNode(Node* node, FBXLoader::Bone* bone, std::map<std::wstring, Node*>& nodeTable)
{
	node->name = bone->name;
	node->meshes = bone->meshes;

	node->transformMatrix = MathConverter::EigenMatrixToXMMatrix(bone->transformMatrix);
	node->offsetMatrix = MathConverter::EigenMatrixToXMMatrix(bone->offsetMatrix);

	nodeTable[node->name] = node;

	for (int i = 0; i < bone->children.size(); i++)
	{
		Node* child = new Node();
		child->parent = node;
		node->children.push_back(child);
		CopyNode(child, bone->children[i], nodeTable);
	}
}
