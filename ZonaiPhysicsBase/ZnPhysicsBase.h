#pragma once


namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnSoftBody;
	class ZnCollider;
	class ZnJoint;


	enum class Shape
	{
		SPHERE,
		BOX,
		CAPSULE,
		PLANE,
		CONVEX,
	};

	class ZnPhysicsBase
	{
	public:
		ZnPhysicsBase() noexcept = default;
		virtual ~ZnPhysicsBase() noexcept;
	
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