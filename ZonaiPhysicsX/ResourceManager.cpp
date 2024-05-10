#include "FBXData.h"
#include "FBXLoader.h"

#include "ResourceManager.h"


namespace ZonaiPhysics
{
	std::unordered_map<ZnMaterialID, physx::PxMaterial*>
	ResourceManager::materialIDTable{};

	std::unordered_map<ZnConvexID, physx::PxConvexMesh*>
	ResourceManager::convexIDTable{};

	std::unordered_map<ZnMeshID, physx::PxTriangleMesh*>
	ResourceManager::triangleIDTable{};

	std::unordered_map<ZnHeightID, physx::PxHeightField*>
	ResourceManager::heightIDTable{};

	void ResourceManager::Initialize()
	{

	}

	void ResourceManager::Release()
	{
		for (auto& material : materialIDTable | std::views::values)
		{
			material->release();
			material = nullptr;
		}

		materialIDTable.clear();

		for (auto& convex : convexIDTable | std::views::values)
		{
			convex->release();
			convex = nullptr;
		}

		convexIDTable.clear();

		for (auto& mesh : triangleIDTable | std::views::values)
		{
			mesh->release();
			mesh = nullptr;
		}

		triangleIDTable.clear();

		for (auto& height : heightIDTable | std::views::values)
		{
			height->release();
			height = nullptr;
		}

		heightIDTable.clear();

	}

	ZnMaterialID ResourceManager::RegistMaterial(physx::PxMaterial* _pxMaterial)
	{
		assert(_pxMaterial != nullptr);

		ZnMaterialID id;
		do
		{
			CreateID(id);
		} while (materialIDTable.contains(id));

		materialIDTable.insert(std::make_pair(id, _pxMaterial));

		return id;
	}

	physx::PxMaterial* ResourceManager::GetPxMaterial(const ZnMaterialID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (materialIDTable.contains(_id))
			return materialIDTable[_id];

		return nullptr;
	}

	void ResourceManager::ReleasePxMaterial(const ZnMaterialID& _id)
	{
		if (materialIDTable.contains(_id))
		{
			const auto material = materialIDTable[_id];
			material->release();
			materialIDTable.erase(_id);
		}
	}

	ZnConvexID ResourceManager::LoadConvex(const std::wstring& _path)
	{
		FBXLoader::FBXLoader loader;

		FBXLoader::Model* model = loader.CreateModelFromFBX(_path);

		using MeshList = std::vector<FBXLoader::Mesh*>;
		MeshList& meshList = model->meshList;

		using Vertices = std::vector<FBXLoader::Vertex>;

		for (int i = 0; i < meshList.size(); i++)
		{
			Vertices& vertices = meshList[i]->vertices;

			for (size_t j = 0; j < vertices.size(); j++)
			{
				vertices[j].position.x;

			}
		}




		loader.ReleaseModel(model);

		return {};
	}

	ZnConvexID ResourceManager::AddConvex(physx::PxConvexMesh* _mesh)
	{
		assert(_mesh != nullptr);

		ZnConvexID id;
		CreateID(id);

		convexIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxConvexMesh* ResourceManager::GetPxConvexShape(const ZnConvexID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (convexIDTable.contains(_id))
			return convexIDTable[_id];

		return nullptr;
	}

	void ResourceManager::ReleaseConvexMesh(const ZnConvexID& _id)
	{
		if (convexIDTable.contains(_id))
		{
			const auto material = convexIDTable[_id];
			material->release();
			convexIDTable.erase(_id);
		}
	}

	ZnMeshID ResourceManager::LoadMesh(const std::wstring& _path)
	{
		return {};
	}

	ZnMeshID ResourceManager::AddMesh(physx::PxTriangleMesh* _mesh)
	{
		assert(_mesh != nullptr);

		ZnMeshID id;
		CreateID(id);

		triangleIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxTriangleMesh* ResourceManager::GetPxMeshShape(const ZnMeshID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (triangleIDTable.contains(_id))
			return triangleIDTable[_id];

		return nullptr;
	}

	void ResourceManager::ReleaseTriangleMesh(const ZnMeshID& _id)
	{
		if (triangleIDTable.contains(_id))
		{
			const auto material = triangleIDTable[_id];
			material->release();
			triangleIDTable.erase(_id);
		}
	}

	ZnHeightID ResourceManager::LoadHeightField(const std::wstring& _path)
	{
		return {};
	}

	ZnHeightID ResourceManager::AddHeightField(physx::PxHeightField* _mesh)
	{
		assert(_mesh != nullptr);

		ZnHeightID id;
		CreateID(id);

		heightIDTable.insert(std::make_pair(id, _mesh));

		return id;
	}

	physx::PxHeightField* ResourceManager::GetPxHeightField(const ZnHeightID& _id)
	{
		if (_id == nullptr)
			return nullptr;

		if (heightIDTable.contains(_id))
			return heightIDTable[_id];

		return nullptr;
	}

	void ResourceManager::ReleaseHeightField(const ZnHeightID& _id)
	{
		if (heightIDTable.contains(_id))
		{
			const auto material = heightIDTable[_id];
			material->release();
			heightIDTable.erase(_id);
		}
	}
}
