#pragma once
#include "ZnRigidBody.h"

namespace ZonaiMath
{
	class Vector3D;
}

namespace physx
{
	class PxRigidDynamic;
}


namespace ZonaiPhysics
{
	using namespace ZonaiMath;



	class RigidBody : public ZnRigidBody
	{
	public:
							RigidBody() noexcept = default;
		virtual				~RigidBody() noexcept;

	public:
		virtual void		WakeUp() noexcept;
		virtual bool		IsSleeping() const noexcept;
		virtual bool		Collide() const noexcept;

	public:
		virtual float		GetLinerDamping() const noexcept;
		virtual float		GetAngularDamping() const noexcept;
		virtual Vector3D	GetLinerVelocity() const noexcept;
		virtual Vector3D	GetAngularVelocity() const noexcept;
		virtual void*		GetUserData() const noexcept;

	public:
		virtual void		SetLinerDamping() noexcept;
		virtual void		SetAngularDamping() noexcept;
		virtual void		SetLinerVelocity() noexcept;
		virtual void		SetAngularVelocity() noexcept;
		virtual void*		SetUserData(void*) noexcept;
		
	public:
		virtual void		AddForce(Vector3D) noexcept;
		virtual void		ClearForce() noexcept;
		virtual void		AddTorque(Vector3D) noexcept;
		virtual void		ClearTorque() noexcept;

	private:
		physx::PxRigidDynamic* rigidbody;
		void* userData;

	public:

	};
}

