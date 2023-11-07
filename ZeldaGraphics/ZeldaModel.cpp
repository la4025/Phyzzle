#include "ZeldaModel.h"

#include "ZeldaTexture.h"
#include "ZeldaMesh.h"
#include "ZeldaCamera.h"
#include "GraphicsResourceID.h"

ZeldaModel::~ZeldaModel()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();

	for (int i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
	materials.clear();

	for (int i = 0; i < zeldaMeshes.size(); i++)
	{
		if (zeldaMeshes[i] != nullptr)
		{
			delete zeldaMeshes[i];
		}
	}
	zeldaMeshes.clear();

	for (int i = 0; i < zeldaTextures.size(); i++)
	{
		if (zeldaTextures[i] != nullptr)
		{
			delete zeldaTextures[i];
		}
	}
	zeldaTextures.clear();

	if (root != nullptr)
	{
		delete root;
	}
	root = nullptr;
}

unsigned int ZeldaModel::GetMeshCount()
{
	return meshes.size();
}

ZeldaMesh* ZeldaModel::GetMesh(unsigned int meshNum)
{
	return zeldaMeshes[meshNum];
}

ZeldaTexture* ZeldaModel::GetTexture(unsigned int meshNum)
{
	return zeldaTextures[meshNum];
}

ZeldaModel::ZeldaModel(ZNode* root, const std::vector<ZMesh*>& meshes, const std::vector<ZMaterial*>& materials, const std::vector<ZeldaMesh*>& zeldaMeshes, const std::vector<ZeldaTexture*>& zeldaTextures) :
	root(nullptr),
	meshes(meshes),
	materials(materials),
	zeldaMeshes(zeldaMeshes),
	zeldaTextures(zeldaTextures)
{

}