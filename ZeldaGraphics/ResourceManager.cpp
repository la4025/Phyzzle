#include "ResourceManager.h"

#include "ZeldaGraphicsDefine.h"
#include "ZeldaModel.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"
#include "ZeldaShader.h"
#include "ZeldaCamera.h"
#include "ZeldaLight.h"
#include "IDGenerator.h"

#include "FBXData.h"
#include "FBXLoader.h"

using namespace DirectX;

void ResourceManager::Initialize(ID3D11Device* device)
{
	this->device = device;
}

MeshID ResourceManager::CreateCubeMesh()
{
	if (cubeID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	std::vector<VertexType> vertexList;
	std::vector<unsigned int> indexList;

	vertexList.resize(24);
	indexList.resize(36);

	XMVECTOR worldUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR worldBack = { 0.0f, 0.0f, 1.0f, 0.0f };

	// 순서 바꾸면 안됨(아래의 Cross계산 때문에)
	XMFLOAT3 dir[6] = {
		{0.0f, 0.0f, -1.0f},
		{0.0f, 0.0f, 1.0f},
		{-1.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f}
	};

	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		XMVECTOR up = worldUp;
		if (faceNum == 4 || faceNum == 5)
		{
			up = worldBack;
		}

		XMVECTOR down = -up;
		XMVECTOR front = XMLoadFloat3(&dir[faceNum]);
		XMVECTOR left = XMVector3Cross(up, XMLoadFloat3(&dir[faceNum]));
		XMVECTOR right = -left;

		// 왼쪽 아래
		XMStoreFloat4(&vertexList[faceNum * 4 + 0].position, 0.5f * (front + left + down));
		vertexList[faceNum * 4 + 0].position.w = 1.0f;
		vertexList[faceNum * 4 + 0].texture = XMFLOAT2(0.0f, 1.0f);
		vertexList[faceNum * 4 + 0].normal = dir[faceNum];
		vertexList[faceNum * 4 + 0].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 0].boneIndices.w = 0xffffffffu;

		// 왼쪽 위
		XMStoreFloat4(&vertexList[faceNum * 4 + 1].position, 0.5f * (front + left + up));
		vertexList[faceNum * 4 + 1].position.w = 1.0f;
		vertexList[faceNum * 4 + 1].texture = XMFLOAT2(0.0f, 0.0f);
		vertexList[faceNum * 4 + 1].normal = dir[faceNum];
		vertexList[faceNum * 4 + 1].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 1].boneIndices.w = 0xffffffffu;

		// 오른쪽 위
		XMStoreFloat4(&vertexList[faceNum * 4 + 2].position, 0.5f * (front + right + up));
		vertexList[faceNum * 4 + 2].position.w = 1.0f;
		vertexList[faceNum * 4 + 2].texture = XMFLOAT2(1.0f, 0.0f);
		vertexList[faceNum * 4 + 2].normal = dir[faceNum];
		vertexList[faceNum * 4 + 2].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 2].boneIndices.w = 0xffffffffu;

		// 오른쪽 아래
		XMStoreFloat4(&vertexList[faceNum * 4 + 3].position, 0.5f * (front + right + down));
		vertexList[faceNum * 4 + 3].position.w = 1.0f;
		vertexList[faceNum * 4 + 3].texture = XMFLOAT2(1.0f, 1.0f);
		vertexList[faceNum * 4 + 3].normal = dir[faceNum];
		vertexList[faceNum * 4 + 3].boneIndices.x = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.y = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.z = 0xffffffffu;
		vertexList[faceNum * 4 + 3].boneIndices.w = 0xffffffffu;
	}

	// Load the index array with data.
	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		indexList[6 * faceNum + 0] = 4 * faceNum + 0;
		indexList[6 * faceNum + 1] = 4 * faceNum + 1;
		indexList[6 * faceNum + 2] = 4 * faceNum + 2;
		indexList[6 * faceNum + 3] = 4 * faceNum + 2;
		indexList[6 * faceNum + 4] = 4 * faceNum + 3;
		indexList[6 * faceNum + 5] = 4 * faceNum + 0;
	}

	cubeID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(cubeID) != 0 || cubeID == MeshID::ID_NULL)
	{
		cubeID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[cubeID] = new ZeldaMesh(device, vertexList, indexList);

	return cubeID;
}

MeshID ResourceManager::CreateSquareMesh()
{
	if (squareID != MeshID::ID_NULL)
	{
		return MeshID::ID_NULL;
	}

	std::vector<VertexType> vertexList(4);
	std::vector<unsigned int> indexList(6);

	vertexList[0] = { { -0.5f, -0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f} };
	vertexList[1] = { { -0.5f, +0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f} };
	vertexList[2] = { { +0.5f, +0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f} };
	vertexList[3] = { { +0.5f, -0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f} };

	indexList[0] = 0;
	indexList[1] = 1;
	indexList[2] = 2;
	indexList[3] = 0;
	indexList[4] = 2;
	indexList[5] = 3;

	squareID = IDGenerator::CreateID<ResourceType::Mesh>();

	while (meshTable.count(squareID) != 0 || squareID == MeshID::ID_NULL)
	{
		squareID = IDGenerator::CreateID<ResourceType::Mesh>();
	}

	meshTable[squareID] = new ZeldaMesh(device, vertexList, indexList);

	return squareID;
}

CameraID ResourceManager::CreateCamera(unsigned int screenWidth, unsigned int screenHeight)
{
	ResourceID cameraID = IDGenerator::CreateID<ResourceType::Camera>();

	while (cameraTable.count(cameraID) != 0 || cameraID == CameraID::ID_NULL)
	{
		cameraID = IDGenerator::CreateID<ResourceType::Camera>();
	}

	cameraTable[cameraID] = new ZeldaCamera(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

	return cameraID;
}

TextureID ResourceManager::CreateTexture(const std::wstring& filePath)
{
	TextureID resourceID = IDGenerator::CreateID<ResourceType::Texture>();

	while (textureTable.count(resourceID) != 0 || resourceID == TextureID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Texture>();
	}

	textureTable[resourceID] = new ZeldaTexture(device, filePath);

	return resourceID;
}

ModelID ResourceManager::CreateModelFromModelingFile(const std::wstring& filePath)
{
	FBXLoader::FBXLoader fbxloader;
	FBXLoader::Model* fbxmodel = fbxloader.CreateModelFromFBX(filePath);

	ModelID resourceID = IDGenerator::CreateID<ResourceType::Model>();

	while (modelTable.count(resourceID) != 0 || resourceID == ModelID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Model>();
	}

	modelTable[resourceID] = new ZeldaModel(device, fbxmodel);

	fbxloader.ReleaseModel(fbxmodel);

	return resourceID;
}

ShaderID ResourceManager::CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath)
{
	ShaderID resourceID = IDGenerator::CreateID<ResourceType::Shader>();

	while (shaderTable.count(resourceID) != 0 || resourceID == ShaderID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Shader>();
	}

	shaderTable[resourceID] = new ZeldaShader(device, vsFilePath, psFilePath);

	return resourceID;
}

LightID ResourceManager::CreateDirectionalLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Directional);

	return resourceID;
}

LightID ResourceManager::CreatePointLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Point);

	return resourceID;
}

LightID ResourceManager::CreateSpotLight()
{
	LightID resourceID = IDGenerator::CreateID<ResourceType::Light>();

	while (lightTable.count(resourceID) != 0 || resourceID == LightID::ID_NULL)
	{
		resourceID = IDGenerator::CreateID<ResourceType::Light>();
	}

	lightTable[resourceID] = new ZeldaLight(LightType::Spot);

	return resourceID;
}

void ResourceManager::ReleaseCubeMesh()
{
	ReleaseMesh(cubeID);
}

void ResourceManager::ReleaseSquareMesh()
{
	ReleaseMesh(squareID);
}

void ResourceManager::ReleaseCamera(CameraID cameraID)
{
	auto& table = cameraTable;
	auto& id = cameraID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseTexture(TextureID textureID)
{
	auto& table = textureTable;
	auto& id = textureID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseModel(ModelID modelID)
{
	auto& table = modelTable;
	auto& id = modelID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseMesh(MeshID meshID)
{
	auto& table = meshTable;
	auto& id = meshID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseShader(ShaderID shaderID)
{
	auto& table = shaderTable;
	auto& id = shaderID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

void ResourceManager::ReleaseLight(LightID lightID)
{
	auto& table = lightTable;
	auto& id = lightID;

	assert(table.count(id) != 0);
	if (table.count(id) != 0)
	{
		auto* targetPtr = table[id];
		if (targetPtr != nullptr)
		{
			delete targetPtr;
		}
		table.erase(id);
	}
}

MeshID ResourceManager::GetCubeID()
{
	return cubeID;
}

MeshID ResourceManager::GetSquareID()
{
	return squareID;
}

ZeldaMesh* ResourceManager::GetCubeMesh()
{
	return meshTable[cubeID];
}

ZeldaMesh* ResourceManager::GetSquareMesh()
{
	return meshTable[squareID];
}

ZeldaCamera* ResourceManager::GetCamera(CameraID cameraID)
{
	auto iter = cameraTable.find(cameraID);

	if (iter != cameraTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaTexture* ResourceManager::GetTexture(TextureID key)
{
	auto iter = textureTable.find(key);

	if (iter != textureTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaModel* ResourceManager::GetModel(ModelID key)
{
	auto iter = modelTable.find(key);

	if (iter != modelTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaMesh* ResourceManager::GetMesh(MeshID key)
{
	auto iter = meshTable.find(key);

	if (iter != meshTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaShader* ResourceManager::GetShader(ShaderID key)
{
	auto iter = shaderTable.find(key);

	if (iter != shaderTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaLight* ResourceManager::GetLight(LightID key)
{
	auto iter = lightTable.find(key);

	if (iter != lightTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

bool ResourceManager::CheckCameraID(CameraID cameraID)
{
	return cameraTable.count(cameraID) != 0;
}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager() :
	device(nullptr),
	cubeID(MeshID::ID_NULL),
	squareID(MeshID::ID_NULL)
{

}

ResourceManager::~ResourceManager()
{
	for (auto iter = modelTable.begin(); iter != modelTable.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = meshTable.begin(); iter != meshTable.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = textureTable.begin(); iter != textureTable.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = shaderTable.begin(); iter != shaderTable.end(); iter++)
	{
		delete iter->second;
	}

	for (auto iter = cameraTable.begin(); iter != cameraTable.end(); iter++)
	{
		delete iter->second;
	}
}