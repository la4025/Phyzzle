#include "ResourceManager.h"

#include "ZeldaGraphicsDefine.h"
#include "ZeldaModel.h"
#include "ZeldaMesh.h"
#include "ZeldaTexture.h"
#include "ZeldaShader.h"
#include "ZeldaCamera.h"
#include "IDGenerator.h"

using namespace DirectX;

void ResourceManager::Initialize(ID3D11Device* device)
{
	this->device = device;
}

bool ResourceManager::CreateMeshWithModelingFile(const std::wstring& filePath)
{
	// not implement
	assert(0);

	return false;
}

bool ResourceManager::CreateCubeMesh()
{
	if (cubeMesh != nullptr)
	{
		return false;
	}

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	vertexCount = 24;
	// Set the number of indices in the index array.
	indexCount = 36;
	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;
	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices) return false;

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
		XMStoreFloat4(&vertices[faceNum * 4 + 0].position, 0.5f * (front + left + down));
		vertices[faceNum * 4 + 0].position.w = 1.0f;
		vertices[faceNum * 4 + 0].texture = XMFLOAT2(0.0f, 1.0f);
		vertices[faceNum * 4 + 0].normal = dir[faceNum];

		// 왼쪽 위
		XMStoreFloat4(&vertices[faceNum * 4 + 1].position, 0.5f * (front + left + up));
		vertices[faceNum * 4 + 1].position.w = 1.0f;
		vertices[faceNum * 4 + 1].texture = XMFLOAT2(0.0f, 0.0f);
		vertices[faceNum * 4 + 1].normal = dir[faceNum];

		// 오른쪽 위
		XMStoreFloat4(&vertices[faceNum * 4 + 2].position, 0.5f * (front + right + up));
		vertices[faceNum * 4 + 2].position.w = 1.0f;
		vertices[faceNum * 4 + 2].texture = XMFLOAT2(1.0f, 0.0f);
		vertices[faceNum * 4 + 2].normal = dir[faceNum];

		// 오른쪽 아래
		XMStoreFloat4(&vertices[faceNum * 4 + 3].position, 0.5f * (front + right + down));
		vertices[faceNum * 4 + 3].position.w = 1.0f;
		vertices[faceNum * 4 + 3].texture = XMFLOAT2(1.0f, 1.0f);
		vertices[faceNum * 4 + 3].normal = dir[faceNum];
	}

	// Load the index array with data.
	for (int faceNum = 0; faceNum < 6; faceNum++)
	{
		indices[6 * faceNum + 0] = 4 * faceNum + 0;
		indices[6 * faceNum + 1] = 4 * faceNum + 1;
		indices[6 * faceNum + 2] = 4 * faceNum + 2;
		indices[6 * faceNum + 3] = 4 * faceNum + 2;
		indices[6 * faceNum + 4] = 4 * faceNum + 3;
		indices[6 * faceNum + 5] = 4 * faceNum + 0;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result)) return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result)) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	cubeMesh = new ZeldaMesh(vertexBuffer, indexBuffer, vertexCount, indexCount);

	return true;
}

ResourceID ResourceManager::CreateTexture(const std::wstring& filePath)
{
	ResourceID resourceID = IDGenerator::CreateID();

	while (textureTable.count(resourceID) != 0 || resourceID == ID_NULL)
	{
		resourceID = IDGenerator::CreateID();
	}

	textureTable[resourceID] = new ZeldaTexture(device, filePath);

	return resourceID;
}

bool ResourceManager::CreateDefaultShader()
{
	if (defaultShader != nullptr)
	{
		return false;
	}

	defaultShader = new ZeldaShader(device, L"CompiledShader\\VertexShader.cso", L"CompiledShader\\PixelShader.cso");
	return true;
}

ResourceID ResourceManager::CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath)
{
	ResourceID resourceID = IDGenerator::CreateID();

	while (shaderTable.count(resourceID) != 0 || resourceID == ID_NULL)
	{
		resourceID = IDGenerator::CreateID();
	}

	shaderTable[resourceID] = new ZeldaShader(device, vsFilePath, psFilePath);

	return resourceID;
}

ResourceID ResourceManager::CreateCamera(unsigned int screenWidth, unsigned int screenHeight)
{
	ResourceID cameraID = IDGenerator::CreateID();

	while (cameraTable.count(cameraID) != 0 || cameraID == ID_NULL)
	{
		cameraID = IDGenerator::CreateID();
	}

	cameraTable[cameraID] = new ZeldaCamera(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

	return cameraID;
}

ZeldaMesh* ResourceManager::GetCubeMesh()
{
	return cubeMesh;
}

ZeldaMesh* ResourceManager::GetMesh(ResourceID key)
{
	auto iter = meshTable.find(key);

	if (iter != meshTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaTexture* ResourceManager::GetTexture(ResourceID key)
{
	auto iter = textureTable.find(key);

	if (iter != textureTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaShader* ResourceManager::GetDefaultShader()
{
	return defaultShader;
}

ZeldaShader* ResourceManager::GetShader(ResourceID key)
{
	auto iter = shaderTable.find(key);

	if (iter != shaderTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

ZeldaCamera* ResourceManager::GetCamera(ResourceID cameraID)
{
	auto iter = cameraTable.find(cameraID);

	if (iter != cameraTable.end())
	{
		return (*iter).second;
	}

	return nullptr;
}

bool ResourceManager::CheckCameraID(ResourceID cameraID)
{
	return cameraTable.count(cameraID) != 0;
}

bool ResourceManager::ReleaseCamera(ResourceID cameraID)
{
	if (cameraTable.count(cameraID) != 0)
	{
		ZeldaCamera* targetCamera = cameraTable[cameraID];
		if (targetCamera != nullptr)
		{
			delete targetCamera;
		}
		cameraTable.erase(cameraID);

		return true;
	}

	return false;
}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager() :
	device(nullptr),
	cubeMesh(nullptr),
	defaultShader(nullptr)
{

}

ResourceManager::~ResourceManager()
{
	if (cubeMesh != nullptr)
	{
		delete cubeMesh;
		cubeMesh = nullptr;
	}

	if (defaultShader != nullptr)
	{
		delete defaultShader;
		defaultShader = nullptr;
	}
}