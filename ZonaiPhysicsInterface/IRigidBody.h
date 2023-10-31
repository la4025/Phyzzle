#pragma once

namespace ZonaiMath
{
	class Vector3D;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;
	
	class IRigidBody
	{

	public:
				IRigidBody() noexcept = default;
		virtual	~IRigidBody() noexcept = 0;

	public:
		virtual void	WakeUp() noexcept = 0;
		virtual bool	isSleeping() const noexcept = 0;

	public:
		/// <summary>
		/// get
		/// </summary>
		virtual float		getLinerDamping() const noexcept = 0;
		virtual float		getAngularDamping() const noexcept = 0;
		virtual Vector3D	getLinerVelocity() const noexcept = 0;
		virtual Vector3D	getAngularVelocity() const noexcept = 0;
		virtual void*		getUserData() const noexcept = 0;

	public:
		/// <summary>
		/// set
		/// </summary>
		virtual void		setLinerDamping() noexcept = 0;
		virtual void		setAngularDamping() noexcept = 0;
		virtual void		setLinerVelocity() noexcept = 0;
		virtual void		setAngularVelocity() noexcept = 0;
		virtual void*		setUserData() const noexcept = 0;
	};
}