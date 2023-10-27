#pragma once


namespace ZonaiPhysics
{
	class IRigidBody;
	class ISoftBody;
	class ICollider;
	class IJoint;


	class ZonaiPhysicsBase
	{
	public:
		ZonaiPhysicsBase() noexcept = default;
		virtual ~ZonaiPhysicsBase() noexcept;
	
	public:
	/// <summary>
	/// 
	/// </summary>
		virtual void			Initialize() noexcept = 0;
		virtual void			Simulation(float _dt) noexcept = 0;
		virtual void			Finalize() noexcept = 0;

	public:
		/// <summary>
		/// Create Component
		/// </summary>
		virtual IRigidBody*		CreateRigidBody() noexcept = 0;
		virtual ISoftBody*		CreateSoftBody() noexcept = 0;
		virtual ICollider*		CreateCollider() noexcept = 0;
		virtual IJoint*			CreateJoint() noexcept = 0;
	};
}