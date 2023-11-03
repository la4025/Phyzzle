#pragma once

#include <string>
#include <unordered_map>

#include <d3d11.h>
#include <DirectXMath.h>

#include "GraphicsResourceID.h"

class ZeldaModel;
class ZeldaMesh;
class ZeldaTexture;
class ZeldaShader;
class ZeldaCamera;

namespace std {
	template <>
	struct hash<std::pair<std::wstring, std::wstring>> {
		std::size_t operator()(const std::pair<std::wstring, std::wstring>& key) const {
			hash<wstring> strHash;
			return strHash(key.first + key.second);
		}
	};
}

namespace std {
	template <>
	struct hash<ResourceID> {
		std::size_t operator()(const ResourceID& key) const {
			return std::hash<unsigned long long>{}(key.ull1) ^ std::hash<unsigned long long>{}(key.ull2);
		}
	};
}

class ResourceManager
{
public:
	void Initialize(ID3D11Device* device);

	bool CreateMeshWithModelingFile(const std::wstring& filePath);
	bool CreateCubeMesh();
	
	ResourceID CreateTexture(const std::wstring& filePath);

	bool CreateDefaultShader();
	ResourceID CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath);

	ResourceID CreateCamera(unsigned int screenWidth, unsigned int screenHeight);

	ZeldaMesh* GetCubeMesh();
	ZeldaMesh* GetMesh(ResourceID key);
	ZeldaTexture* GetTexture(ResourceID key);
	ZeldaShader* GetDefaultShader();
	ZeldaShader* GetShader(ResourceID key);
	ZeldaCamera* GetCamera(ResourceID cameraID);

	bool CheckCameraID(ResourceID cameraID);


	bool ReleaseCamera(ResourceID cameraID);

private:
	ID3D11Device* device;
	std::unordered_map<ResourceID, ZeldaMesh*> meshTable;
	std::unordered_map<ResourceID, ZeldaTexture*> textureTable;
	std::unordered_map<ResourceID, ZeldaShader*> shaderTable;

	std::unordered_map<ResourceID, ZeldaCamera*> cameraTable;

	ZeldaMesh* cubeMesh;
	ZeldaShader* defaultShader;

	//singleton
public:
	static ResourceManager& GetInstance();

private:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager& ref) = delete;
	ResourceManager& operator=(const ResourceManager& ref) = delete;
};