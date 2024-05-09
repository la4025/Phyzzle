#pragma once
#include "EventCallback.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "ConvexCollider.h"

#include "MaterialEnum.h"
#include "ZnMaterial.h"
#include "PxPhysicsAPI.h"
#include "omnipvd/PxOmniPvd.h"

#define PX_SUPPORT_PVD 1
#define PX_SUPPORT_OMNI_PVD 0

#if PX_SUPPORT_OMNI_PVD
#include "../pvdruntime/include/OmniPvdWriter.h"
#include "../pvdruntime/include/OmniPvdFileWriteStream.h"
#endif

#pragma warning (pop)

namespace physx
{
	class PxPhysics;
}

namespace ZonaiPhysics
{
	class ZnSimulationCallback;

	class RigidBody;

	class Collider;
	class BoxCollider;
	class SphereCollider;
	class CapsuleCollider;
	class ConvexCollider;
	class MeshCollider;

	class Joint;
	class FixedJoint;
	class PrismaticJoint;
	class DistanceJoint;
	class SphericalJoint;
	class HingeJoint;

	class ZnTransform;
}

namespace ZonaiPhysics
{
	template <typename Ty>
	concept znCollider = std::is_base_of_v<ZnCollider, Ty>;

	class ZnFactoryX
	{
	public:
		static void CreatePhysxFactory();
		static void PhysicsWithOmniPvd();
		static void SetSimulationCallback(ZnSimulationCallback* _instance);
		static void Release();

	public:
		static physx::PxScene*			CreateScene(void* _userScene, const Eigen::Vector3f& _gravity);
		static physx::PxMaterial*		CreateMaterial(const MaterialDesc& _desc);

		/// rigidbody
		static RigidBody*				CreateDynamicRigidBody(void* _userData);
		static RigidBody*				CreateStaticRigidBody(void* _userData);


		static physx::PxShape*			CreateBoxShape(const Eigen::Vector3f& _extend, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateSphereShape(float _radius, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateCapsuleShape(float _radius, float _height, const physx::PxMaterial* _material);
		static physx::PxShape*			CreateTriagleMesh(
			physx::PxTriangleMesh* _mesh, 
			const Eigen::Vector3f& _scale, 
			const Eigen::Quaternionf& _rotation, 
			const physx::PxMaterial* _material
		);
		static physx::PxShape*			CreateConvexMesh(
			physx::PxConvexMesh* _mesh,
			const Eigen::Vector3f& _scale,
			const Eigen::Quaternionf& _rotation,
			const physx::PxMaterial* _material
		);

		static physx::PxTriangleMesh*	CookTriagleMesh(const physx::PxMaterial* _material);
		static physx::PxConvexMesh*		CookConvexMesh(const physx::PxMaterial* _material);

		/// collider
		template <znCollider collider>
		static collider*				CreateZnCollider(void* _znBody, physx::PxShape* _pxShape)
		{
			assert(_znBody != nullptr);
			assert(_pxShape != nullptr);

			const auto znBody = static_cast<RigidBody*>(_znBody);

			const auto zn_collider = new collider(_pxShape, znBody);
			RigidBodyHelper::Attach(znBody->pxBody, _pxShape);

			return zn_collider;
		}

		static MeshCollider*			CreateMeshCollider(void* _znBody , const physx::PxMaterial* _material);
		static ConvexCollider*			CreateConvexCollider(void* _znBody , const physx::PxMaterial* _material);

		/// joint
		static FixedJoint*				CreateFixedJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static PrismaticJoint*			CreatePrismaticJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static DistanceJoint*			CreateDistanceJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static SphericalJoint*			CreateSphericalJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);
		static HingeJoint*				CreateHingeJoint(RigidBody* _userData0, const ZnTransform& tm0, RigidBody* _userData1, const ZnTransform& tm1);

	private:
		static ZonaiPhysics::EventCallback eventCallback;
		static physx::PxDefaultAllocator allocator;
		static physx::PxDefaultErrorCallback  errorCallback;
		static physx::PxDefaultCpuDispatcher* dispatcher;
		static physx::PxFoundation* foundation;
		static physx::PxPhysics* pxFactory;
		static physx::PxPvd* pxPvd;
		static physx::PxOmniPvd* omniPvd;
		// static PxCooking* pxCooking;
	};
}