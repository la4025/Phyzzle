#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#include <map>
#include <string>

#include "EventCallback.h"
#include "FilterCallback.h"

#define PVD_HOST "127.0.0.1"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxPvd;
	class PxRigidDynamic;
}

namespace ZonaiPhysics
{
	class ZnPhysicsX;

	class ZnRigidBody;
	class RigidBody;

	class ZnSoftBody;
	class SoftBody;

	class ZnCollider;
	class Collider;

	class ZnJoint;
	class Joint;
	class FixedJoint;

	class ZnTransform;

	struct ZnRaycastInfo;
	struct ZnMaterial;

	class ZnPhysicsX : public ZnPhysicsBase
	{
	public:
		ZnPhysicsX() noexcept = default;
		~ZnPhysicsX() noexcept override = default;

	public:
		void Initialize(ZnSimulationCallback*) noexcept override;
		void Simulation(float _dt) noexcept override;
		void Finalize() noexcept override;

		static ZnPhysicsBase* Instance();

	public:
		void SetGravity(const Vector3f&) noexcept override;
		void SetLayerData() noexcept;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		ZnRigidBody* CreateRigidBody(const std::wstring&) noexcept override;

		/// <summary>
		/// Create Collider
		/// </summary>
		ZnCollider* CreateBoxCollider(const std::wstring&, float x, float y, float z) noexcept override;
		ZnCollider* CreateSphereCollider(const std::wstring&, float radius) noexcept override;
		ZnCollider* CreateCapsuleCollider(const std::wstring&, float radius, float height) noexcept override;
		// ZnCollider*				CreateCustomCollider(const std::wstring&) noexcept override;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 고정 조인트
		ZnFixedJoint*		CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
		// 거리 조인트
		ZnDistanceJoint*	CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
		// 구형 조인트
		ZnSphericalJoint*	CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
		// 회전 조인트
		ZnHingeJoint*		CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;
		// 프리즘 조인트
		ZnPrismaticJoint*	CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*, const ZnTransform&) noexcept override;

	public:
		bool Raycast(const Vector3f&, const Vector3f&, float, ZnRaycastInfo&) noexcept override;

	private:
		RigidBody* FindRigidBody(const std::wstring&) noexcept;
		Collider* CreateCollider(const std::wstring&) noexcept;

	private:
		physx::PxDefaultAllocator allocator;
		physx::PxDefaultErrorCallback errorCallback;
		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxDefaultCpuDispatcher* dispatcher;
		physx::PxScene* scene;
		physx::PxPvd* pvd;

		physx::PxMaterial* material;

	private:
		static ZnPhysicsX* instance;

		std::map<std::wstring, RigidBody*> bodies;
		EventCallback eventCallback;
		FilterCallback filterCallback;
	};


	extern "C" {
	__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
	}
} // namespace ZonaiPhysics
