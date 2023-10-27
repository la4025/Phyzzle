#pragma once

namespace ZonaiPhysics
{
	class IRigidBody
	{
	public:
		IRigidBody() noexcept = default;
		virtual ~IRigidBody() noexcept = 0;

	public:
		virtual void	WakeUp() noexcept = 0;
		virtual bool	isSleeping() const noexcept = 0;

	public:
		/// <summary>
		/// get
		/// </summary>
		virtual float	getLinerVelocity() const noexcept = 0;
		virtual float	getAngularVelocity() const noexcept = 0;
		virtual void*	getUserData() const noexcept = 0;

	public:
		/// <summary>
		/// set
		/// </summary>
		virtual void	setLinerVelocity() noexcept = 0;
		virtual void	setAngularVelocity() noexcept = 0;
		virtual void*	setUserData() const noexcept = 0;
	};
}