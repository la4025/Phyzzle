#pragma once
#include <functional>
#include <vector>
#include <windows.h>
#include "ZnRaycastInfo.h"

namespace ZonaiPhysics
{
	class Joint;
	class ZnPhysicsBase;
	class ZnRigidBody;
	class ZnCollider;
	class ZnJoint;
}

namespace PurahEngine
{
	class EventCallbackSystem;
	class Collider;
	class RigidBody;
	class Joint;

	class PhysicsSystem
	{
	public:
		PhysicsSystem() noexcept = default;
		~PhysicsSystem() noexcept = default;

		PhysicsSystem(const PhysicsSystem&) noexcept = delete;
		PhysicsSystem(PhysicsSystem&&) noexcept = delete;

		PhysicsSystem& operator=(const PhysicsSystem&) noexcept = delete;
		PhysicsSystem& operator=(PhysicsSystem&&) noexcept = delete;

	private:
		ZonaiPhysics::ZnPhysicsBase* physics;
		HMODULE ZonaiPhysicsXDLL;
		std::function<void()> releaseFuntion;

	public:
		void Initialize() noexcept;
		void PreStep() const;
		void Simulation(float _dt) const noexcept;
		void SimulateResult() const;
		void Finalize() noexcept;

	public:
		void FreeObject(ZonaiPhysics::ZnRigidBody*, void* _gameObject) const;
		void FreeObject(ZonaiPhysics::ZnCollider*, void* _gameObject) const;
		void FreeObject(ZonaiPhysics::ZnJoint*, void* _gameObject) const;

	public:
		std::vector<RigidBody*> bodies;
		std::vector<Collider*> dynamicColliders;
		std::vector<Collider*> staticColliders;
		std::vector<Joint*> joints;
		EventCallbackSystem* callbackSystem;

	public:
		ZonaiPhysics::ZnRigidBody*	CreateRigidBody(void* _gameObject) const noexcept;

	public:
		ZonaiPhysics::ZnCollider*	CreateBoxCollider(void* _gameObject, float x, float y, float z) const noexcept;
		// ZonaiPhysics::ZnCollider*	CreatPlaneCollider(void* _gameObject, float x, float y) noexcept;
		ZonaiPhysics::ZnCollider*	CreateSphereCollider(void* _gameObject, float radius) const noexcept;
		ZonaiPhysics::ZnCollider*	CreateCapsuleCollider(void* _gameObject, float radius, float height) const noexcept;
		// ZonaiPhysics::ZnCollider*	CreateCustomCollider(void* _gameObject) noexcept;

	public:
		// ZonaiPhysics::ZnJoint*		CreateFixedJoint(
		// 	ZonaiPhysics::ZnRigidBody* , Transform*, 
		// 	ZonaiPhysics::ZnRigidBody*, Transform*);
		// ZonaiPhysics::ZnJoint*		CreateHingeJoint();
		// ZonaiPhysics::ZnJoint*		CreateBallJoint();
		// ZonaiPhysics::ZnJoint*		CreateDistanceJoint();
		// ZonaiPhysics::ZnJoint*		CreateSpringJoint();

	public:
		bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distance, ZonaiPhysics::ZnRaycastInfo& _out);

	public:
		static PhysicsSystem& GetInstance();
	};
}
