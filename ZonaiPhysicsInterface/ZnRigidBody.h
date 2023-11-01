#pragma once

namespace ZonaiMath
{
	class Vector3D;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;
	
	class ZnRigidBody
	{

	public:
							ZnRigidBody() noexcept = default;
		virtual				~ZnRigidBody() noexcept = 0;

	public:
		virtual void		WakeUp() noexcept = 0;
		virtual bool		IsSleeping() const noexcept = 0;
		virtual bool		Collide() const noexcept = 0;

	public:
		/// <summary>
		/// get
		/// </summary>
		virtual float		GetLinerDamping() const noexcept = 0;
		virtual float		GetAngularDamping() const noexcept = 0;
		virtual Vector3D	GetLinerVelocity() const noexcept = 0;
		virtual Vector3D	GetAngularVelocity() const noexcept = 0;
		virtual void*		GetUserData() const noexcept = 0;

	public:
		/// <summary>
		/// set
		/// </summary>
		virtual void		SetLinerDamping() noexcept = 0;
		virtual void		SetAngularDamping() noexcept = 0;
		virtual void		SetLinerVelocity() noexcept = 0;
		virtual void		SetAngularVelocity() noexcept = 0;
		virtual void*		SetUserData(void*) const noexcept = 0;

	public:
		virtual void		AddForce(Vector3D) noexcept = 0;
		virtual void		ClearForce() noexcept = 0;
		virtual void		AddTorque(Vector3D) noexcept = 0;
		virtual void		ClearTorque() noexcept = 0;
	};
}