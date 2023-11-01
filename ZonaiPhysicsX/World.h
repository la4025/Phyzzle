#pragma once
#include "ZonaiPhysicsBase.h"

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;
	class ZnTransform;


	class World : public ZonaiPhysicsBase
	{
	public:
		World() noexcept = default;
		virtual ~World() noexcept;

	public:
		virtual void		Initialize() noexcept;
		virtual void		Finalize() noexcept;

	public:
		/// <summary>
		/// Create RigidBoby
		/// </summary>
		virtual ZnRigidBody* CreateRigidBody() noexcept;
		
		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ZnCollider*	CreatBoxCollider() noexcept;
		virtual ZnCollider*	CreatPlaneCollider() noexcept;
		virtual ZnCollider*	CreatSphereCollider() noexcept;
		virtual ZnCollider*	CreateCapsuleCollider() noexcept;
		virtual ZnCollider*	CreateCustomCollider() noexcept;
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual ZnJoint*		CreatD6Joint() noexcept;			// D6 조인트		*사실 뭔지 모름
		virtual ZnJoint*		CreatFixedJoint(ZnRigidBody*, ZnTransform, ZnRigidBody*, ZnTransform) noexcept;			// 고정 조인트
		virtual ZnJoint*		CreatDistanceJoint() noexcept;		// 거리 조인트
		virtual ZnJoint*		CreatSphericalJoint() noexcept;		// 구형 조인트
		virtual ZnJoint*		CreatRevoluteJoint() noexcept;		// 회전 조인트
		virtual ZnJoint*		CreatPrismaticJoint() noexcept;		// 프리즘 조인트
	};
}

