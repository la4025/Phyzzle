#include "ResourceManager.h"

#include "../ZeldaFBXLoader/FBXData.h"
#include "../ZeldaFBXLoader/FBXLoader.h"

namespace ZonaiPhysics
{
	FBXLoader::Model* ResourceManager::LoadFBXFile(const std::wstring& _path)
	{
		FBXLoader::FBXLoader loader;

		return loader.CreateModelFromFBX(_path);
	}

	void ResourceManager::UnloadFBXFile(const std::wstring& _path)
	{

	}
}
