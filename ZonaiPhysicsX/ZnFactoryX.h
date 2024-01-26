#pragma once
#include "ZnCollider.h"
#include "ZnJoint.h"

#include "ZnMaterial.h"
#include "ZnTransform.h"

#include "PxPhysicsAPI.h"

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class CapsuleCollider;
	class BoxCollider;
}

namespace ZonaiPhysics
{
	class SphereCollider;
	class Joint;
	class Collider;
	class RigidBody;

	class ZnFactoryX
	{
	public:
		static void CreatePhysxFactory();
		static void Release();

	private:
		static physx::PxScene*			CreateScene(const physx::PxSceneDesc&);
		static physx::PxMaterial*		CreateMaterial(float _staticFriction, float _dynamicFriction, float _restitution);

		static physx::PxRigidDynamic*	CreateRigidBody(void*);

		static physx::PxShape*			CreateBoxCollider(void*, float, float, float);
		static physx::PxShape*			CreateSphereCollider(void*, float);
		static physx::PxShape*			CreateCapsuleCollider(void*, float, float);

		static physx::PxFixedJoint*		CreateFixedJoint();
		static physx::PxPrismaticJoint*	CreatePrismaticJoint();
		static physx::PxDistanceJoint*	CreateDistanceJoint();
		static physx::PxSphericalJoint*	CreateSphericalJoint();
		static physx::PxRevoluteJoint*	CreateRevoluteJointt();

		template <typename JointT> requires (std::is_base_of_v<ZnJoint, JointT>)
		static JointT* CreateJoint(void* _userData0, void* _userData1, const ZnTransform& _tm0, const ZnTransform& _tm1)
		{
			Joint* joint = new JointT(_userData0, _userData1, _tm0, _tm1);

			return joint;
		}

	private:
		static physx::PxDefaultAllocator allocator;
		static physx::PxDefaultErrorCallback  errorCallback;
		static physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
		static physx::PxFoundation* foundation = nullptr;
		static physx::PxPhysics* pxFactory = nullptr;
		static physx::PxPvd* pxPvd = nullptr;
	};
}

/*
 * PhysX의 객체를 생성을 도와주는 클래스
 *
 * Rigidbody나 Joint를 이 친구가 만들어줌
 *
 */