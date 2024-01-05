#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

#include <d3d11.h>
#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GraphicsResourceID.h"

class ZeldaModel;
class ZeldaMesh;
class ZeldaTexture;
class ZeldaShader;
class ZeldaCamera;

struct ZNode;
struct ZMesh;

namespace std {
	template <>
	struct hash<std::pair<std::wstring, std::wstring>> {
		std::size_t operator()(const std::pair<std::wstring, std::wstring>& key) const {
			hash<wstring> strHash;
			return strHash(key.first + key.second);
		}
	};
}

class ResourceManager
{
public:
	void Initialize(ID3D11Device* device);

	ModelID CreateModelFromModelingFile(const std::wstring& filePath);

	bool CreateCubeMesh();
	MeshID GetCubeID();
	
	TextureID CreateTexture(const std::wstring& filePath);

	ShaderID CreateShader(const std::wstring& vsFilePath, const std::wstring& psFilePath);

	CameraID CreateCamera(unsigned int screenWidth, unsigned int screenHeight);

	ZeldaModel* GetModel(ModelID key);
	ZeldaMesh* GetCubeMesh();
	ZeldaMesh* GetMesh(MeshID key);
	ZeldaTexture* GetTexture(TextureID key);
	ZeldaShader* GetShader(ShaderID key);
	ZeldaCamera* GetCamera(CameraID cameraID);

	bool CheckCameraID(CameraID cameraID);


	bool ReleaseCamera(CameraID cameraID);

private:
	ID3D11Device* device;
	std::unordered_map<ModelID, ZeldaModel*> modelTable;
	std::unordered_map<MeshID, ZeldaMesh*> meshTable;
	std::unordered_map<TextureID, ZeldaTexture*> textureTable;
	std::unordered_map<ShaderID, ZeldaShader*> shaderTable;

	std::unordered_map<CameraID, ZeldaCamera*> cameraTable;

	MeshID cubeID;

	//singleton
public:
	static ResourceManager& GetInstance();

private:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager& ref) = delete;
	ResourceManager& operator=(const ResourceManager& ref) = delete;
};