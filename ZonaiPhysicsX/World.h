#pragma once
#include "ZnPhysicsBase.h"
#include "PxPhysicsAPI.h"
<<<<<<< HEAD
#include <map>
#include <string>

#define PVD_HOST "127.0.0.1"
=======
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe

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
	class ZnRigidBody;
	class RigidBody;
	class ZnSoftBody;
	class SoftBody;
	class ZnCollider;
	class Collider;
	class ZnJoint;
	class Joint;
	class ZnTransform;
	class World;

	extern "C"
	{
		__declspec(dllexport) ZnPhysicsBase* CreatePhysics() noexcept;
	}

	class World : public ZnPhysicsBase
	{
	public:
		World() noexcept = default;
		virtual ~World() noexcept;

	public:
		virtual void			Initialize() noexcept override;
		virtual void			Simulation(float _dt) noexcept override;
		virtual void			Finalize() noexcept override;

	public:
		/// <summary>
		/// Create Component
		/// </summary>
		virtual ZnRigidBody*	CreateRigidBody() noexcept override;
		virtual ZnSoftBody*		CreateSoftBody() noexcept override;
		virtual ZnCollider*		CreateCollider() noexcept override;
		virtual ZnJoint*		CreateJoint() noexcept override;


		/// <summary>
		/// Create RigidBoby
		/// </summary>
<<<<<<< HEAD
		virtual ZnRigidBody*	CreateRigidBody(const std::wstring&) noexcept override;
=======
		virtual ZnRigidBody*	CreateRigidBody() noexcept;
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
		
		/// <summary>
		/// Create Collider
		/// </summary>
<<<<<<< HEAD
		virtual ZnCollider*		CreatBoxCollider(const std::wstring&, float width, float height) noexcept;
		virtual ZnCollider*		CreatPlaneCollider(const std::wstring&) noexcept;
		virtual ZnCollider*		CreatSphereCollider(const std::wstring&) noexcept;
		virtual ZnCollider*		CreateCapsuleCollider(const std::wstring&) noexcept;
		virtual ZnCollider*		CreateCustomCollider(const std::wstring&) noexcept;
=======
		virtual ZnCollider*		CreatBoxCollider(float width, float height) noexcept;
		virtual ZnCollider*		CreatPlaneCollider() noexcept;
		virtual ZnCollider*		CreatSphereCollider() noexcept;
		virtual ZnCollider*		CreateCapsuleCollider() noexcept;
		virtual ZnCollider*		CreateCustomCollider() noexcept;
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual ZnJoint*		CreatD6Joint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// D6 조인트		*사실 뭔지 모름
		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// 고정 조인트
		virtual ZnJoint*		CreatDistanceJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// 거리 조인트
		virtual ZnJoint*		CreatSphericalJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// 구형 조인트
		virtual ZnJoint*		CreatRevoluteJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// 회전 조인트
		virtual ZnJoint*		CreatPrismaticJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;		// 프리즘 조인트
	
	private:
		physx::PxDefaultAllocator		allocator;
		physx::PxDefaultErrorCallback	errorCallback;
		physx::PxFoundation*			foundation;
		physx::PxPhysics*				physics;
		physx::PxDefaultCpuDispatcher*	dispatcher;
		physx::PxScene*					scene;
		physx::PxMaterial*				material;
		physx::PxPvd*					pvd;
<<<<<<< HEAD

	private:
		std::map<std::wstring, RigidBody*> bodies;
		bool first;
=======
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
	};
} // namespace ZonaiPhysics

