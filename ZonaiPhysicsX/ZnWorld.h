#pragma once
#include <map>
#include <set>
#include <vector>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo;
	class EventCallback;
	class ZnTransform;
	class RigidBody;
}

namespace physx
{
	class PxPhysics;
	class PxScene;
}

namespace ZonaiPhysics
{
	class ZnWorld
	{
	public:
		static void					Run(float _dt);
		static void					Release();

	public:
		static void					AddScene(void* _key, physx::PxScene* _scene);
		static void					LoadScene(void* _key);
		static void					UnloadScene(void* _key);

		static void					SetGravity(const Vector3f&, void* _scene);
		static bool					Raycast(const Vector3f& _from, const Vector3f& _to, float _distance,
											ZnRaycastInfo& _out);

		static bool					Boxcast(float _x, float _y, float _z, const ZnTransform& trans);

	public:
		static void					AddBody(void* _znBody, void* _scene);
		static RigidBody*			GetBody(void* _znBody, void* _userScene);

		static void					AddMaterial(void*);
		static physx::PxMaterial*	GetMaterial(void*);
		

		// static bool Gometrycast();

	private:
		using BodyList = std::vector<RigidBody*>;

		static physx::PxScene* currScene;
		static std::unordered_map<void*, physx::PxScene*> sceneList;
		static std::map<physx::PxScene*, BodyList> bodies;
		static std::set<physx::PxMaterial*> materials;
	};
}


