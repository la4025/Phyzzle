#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <string>
#include <queue>

#include "ModelingData.h"
#include "ZeldaGraphicsDefine.h"

class ZeldaMesh;
class ZeldaTexture;

class ZeldaModel
{
public:
	~ZeldaModel();

	std::vector<ZNode*> GetAllNode();
	unsigned int GetMeshCount();
	ZeldaMesh* GetMesh(unsigned int meshNum);
	ZeldaTexture* GetTexture(unsigned int meshNum);

private:
	ZeldaModel(ZNode* root, const std::vector<ZMesh*>& meshes, const std::vector<ZMaterial*>& materials, const std::vector<ZeldaMesh*>& zeldaMeshes, const std::vector<ZeldaTexture*>& zeldaTextures);
	ZeldaModel(const ZeldaModel& zeldaModel) = delete;

	ZNode* root;
	std::vector<ZMesh*> meshes;
	std::vector<ZMaterial*> materials;

	std::vector<ZeldaMesh*> zeldaMeshes;
	std::vector<ZeldaTexture*> zeldaTextures;

	friend class ResourceManager;
};