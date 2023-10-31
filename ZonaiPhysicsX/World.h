#pragma once
#include "ZonaiPhysicsBase.h"

namespace ZonaiPhysics
{
	class IRigidBody;
	class ISoftBody;
	class ICollider;
	class IJoint;


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
		virtual IRigidBody* CreateRigidBody() noexcept;
		
		/// <summary>
		/// Create Collider
		/// </summary>
		virtual ICollider*	CreatBoxCollider() noexcept;
		virtual ICollider*	CreatPlaneCollider() noexcept;
		virtual ICollider*	CreatSphereCollider() noexcept;
		virtual ICollider*	CreateCapsuleCollider() noexcept;
		virtual ICollider*	CreateCustomCollider() noexcept;
		
		/// <summary>
		/// Create Joint
		/// </summary>
		virtual IJoint*		CreatD6Joint() noexcept;
		virtual IJoint*		CreatFixedJoint() noexcept;
		virtual IJoint*		CreatDistanceJoint() noexcept;
		virtual IJoint*		CreatSphericalJoint() noexcept;
		virtual IJoint*		CreatRevoluteJoint() noexcept;
		virtual IJoint*		CreatPrismaticJoint() noexcept;
	};
}

