#pragma once
#include <map>
#include <vector>
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	struct ZnRaycastInfo;
	class EventCallback;
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
		static void				Run(float _dt);

		static physx::PxScene*	CreateScene(physx::PxPhysics** _physx, physx::PxDefaultCpuDispatcher** _dispatcher, EventCallback* _eventCallback);
		static void				AddScene(physx::PxScene*);

		static void				AddBody(void* _scene, void* _body);
		static void				SetGravity(void* _scene, const Eigen::Vector3f&);
		static bool				Raycast(void* _scene, const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZnRaycastInfo& _out);

		static void				AddRigidBody(RigidBody* _body);

	private:
		using BodyList = std::vector<RigidBody*>;

		static physx::PxScene* currScene;
		static std::map<physx::PxScene*, BodyList> sceneList;
	};
}

/*
 * Scene을 가진 클래스
 * Raycast와 해당 씬의 RigidBody를 가지고 있음.
 * 
 */