#pragma once
#include <string>
#include <unordered_map>
#include "ZnResourceID.h"

namespace physx
{
	class PxMaterial;
}

namespace FBXLoader
{
	struct Model;
}

namespace ZonaiPhysics
{
	class ResourceManager
	{
	private:
		// static std::unordered_map<ResourceID, FBXLoader::Model*>  loadedResources;

	public:
		static ZnMaterialID			CreateMaterial(const std::wstring& _path);
		static physx::PxMaterial*	GetPxMaterial(const ZnMaterialID& _material);
		static void					ReleasePxMaterial(const ZnMaterialID& _material);

		static ZnConvexID			LoadConvex(const std::wstring& _path);
		static void					UnloadConvex(ZnConvexID);

		static ZnMeshID				LoadMesh(const std::wstring& _path);
		static void					UnloadMesh(ZnMeshID);

		// static ResourceID LoadFBXFile(const std::wstring& _path);
		// static FBXLoader::Model* GetModel(ResourceID resourceID);
		// static void UnloadFBXFile(ResourceID resourceID);
	};
}
