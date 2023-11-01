#pragma once


namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;


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
		virtual ZnRigidBody*	CreateRigidBody() noexcept = 0;
		virtual ZnSoftBody*		CreateSoftBody() noexcept = 0;
		virtual ZnCollider*		CreateCollider() noexcept = 0;
		virtual ZnJoint*		CreateJoint() noexcept = 0;
	};
}