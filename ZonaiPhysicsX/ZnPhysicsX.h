#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#include <map>
#include <set>
#include <string>

#include "EventCallback.h"
#include "FilterCallback.h"
#include "ZnFactoryX.h"
#include "ZnLayer.h"

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

		static ZnPhysicsX* Instance();

	public:
		void CreateScene(void* _userScene, const Vector3f& _gravity) noexcept override;
		void LoadScene(void* _userScene) noexcept override;
		void UnloadScene(void* _userScene) noexcept override;

		void SetGravity(const Vector3f&, void* _userScene = nullptr) noexcept override;
		void SetCollisionLayerData(uint32_t _layer, const std::initializer_list<uint32_t>& _data) noexcept;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		ZnRigidBody* CreateRigidBody(void* _userData, void* _scene = nullptr) noexcept override;

		/// <summary>
		/// Create Collider
		/// </summary>
		ZnCollider* CreateBoxCollider(void* _userData, float x, float y, float z) noexcept override;
		ZnCollider* CreateSphereCollider(void* _userData, float radius) noexcept override;
		ZnCollider* CreateCapsuleCollider(void* _userData, float radius, float height) noexcept override;

		/// <summary>
		/// Create Joint
		/// </summary>
		// 고정 조인트
		ZnFixedJoint* CreateFixedJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*,
		                               const ZnTransform&) noexcept override;
		// 거리 조인트
		ZnDistanceJoint* CreateDistanceJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*,
		                                     const ZnTransform&) noexcept override;
		// 구형 조인트
		ZnSphericalJoint* CreateSphericalJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*,
		                                       const ZnTransform&) noexcept override;
		// 회전 조인트
		ZnHingeJoint* CreateHingeJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*,
		                               const ZnTransform&) noexcept override;
		// 프리즘 조인트
		ZnPrismaticJoint* CreatePrismaticJoint(ZnRigidBody*, const ZnTransform&, ZnRigidBody*,
		                                       const ZnTransform&) noexcept override;

	public:
		bool Raycast(const Vector3f&, const Vector3f&, float, ZnRaycastInfo&) noexcept override;
		// bool Boxcast(const Vector3f&, const Vector3f&, float, ZnRaycastInfo&) noexcept override;

	private:
		physx::PxMaterial* defaultMaterial;

	private:
		static ZnPhysicsX* instance;
	};


	extern "C" {
	__declspec(dllexport) ZnPhysicsBase* CreatePhysics();
	}
} // namespace ZonaiPhysics
