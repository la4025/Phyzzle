#include <cassert>
#include "../ZonaiPhysicsBase/ZnPhysicsBase.h"

#include "EventCallbackSystem.h"
#include "Collider.h"
#include "RigidBody.h"
#include "ZnJoint.h"

#include "PhysicsSystem.h"

namespace PurahEngine
{
	void PhysicsSystem::Initialize() noexcept
	{
		ZonaiPhysicsXDLL = LoadLibrary(L"ZonaiPhysicsX.dll");

		if (ZonaiPhysicsXDLL == nullptr)
		{
			// DLL 로드 실패
			assert(0);
		}

		using ImportFunction = ZonaiPhysics::ZnPhysicsBase* (*) ();
		ImportFunction createZonaiPhysics{ reinterpret_cast<ImportFunction>(GetProcAddress(ZonaiPhysicsXDLL, "CreatePhysics")) };


		if (createZonaiPhysics == nullptr)
		{
			// DLL 함수를 찾을 수 없습니다.
			assert(0);
		}

		releaseFuntion = { reinterpret_cast<void (*) ()>(GetProcAddress(ZonaiPhysicsXDLL, "ReleasePhysics")) };

		if (releaseFuntion == nullptr)
		{
			// DLL 함수를 찾을 수 없습니다.
			assert(0);
		}

		physics = createZonaiPhysics();

		if (physics == nullptr)
		{
			assert(0);
		}

		callbackSystem = new EventCallbackSystem;

		physics->Initialize(callbackSystem);
		physics->CreateScene(this, {0, -9.81, 0});
		physics->LoadScene(this);
	}

	void PhysicsSystem::PreStep() const
	{
		for (const auto& e : dynamicColliders)
		{
			e->PreStep();
		}
	}

	void PhysicsSystem::Simulation(float _dt) const noexcept
	{
		physics->Simulation(_dt);
	}

	void PhysicsSystem::SimulateResult() const
	{
		for (const auto& e : bodies)
		{
			e->SimulateResult();
		}
	}

	void PhysicsSystem::Finalize() const noexcept
	{
		physics->Finalize();

		/// Release 함수
		releaseFuntion();

		FreeLibrary(ZonaiPhysicsXDLL);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnRigidBody* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseRigidBody(_object, _gameObject);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnCollider* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseCollider(_object, _gameObject);
	}

	void PhysicsSystem::FreeObject(ZonaiPhysics::ZnJoint* _object, void* _gameObject) const
	{
		assert(_object != nullptr);

		physics->ReleaseJoint(_object, _gameObject);
	}

	ZonaiPhysics::ZnRigidBody* PhysicsSystem::CreateRigidBody(
		void* _gameObject) const noexcept
	{
		return physics->CreateRigidBody(_gameObject);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateBoxCollider(
		void* _gameObject, float x, float y, float z) const noexcept
	{
		return physics->CreateBoxCollider(_gameObject, { x, y, z }, 0);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateSphereCollider(
		void* _gameObject, float radius) const noexcept
	{
		return physics->CreateSphereCollider(_gameObject, radius, 0);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(
		void* _gameObject, float radius, float height) const noexcept
	{
		return physics->CreateCapsuleCollider(_gameObject, radius, height, 0);
	}

	// ZonaiPhysics::ZnJoint* PhysicsSystem::CreateFixedJoint(
	// 	ZonaiPhysics::ZnRigidBody*, Transform*,
	// 	ZonaiPhysics::ZnRigidBody*, Transform*)
	// {
	// 	// return physics->CreateFixedJoint()
	// 	return nullptr;
	// }

	bool PhysicsSystem::Raycast(
		const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, 
		float _distance, ZonaiPhysics::ZnRaycastInfo& _out)
	{
		return physics->Raycast(_from, _to, _distance, _out);
	}

	PurahEngine::PhysicsSystem& PurahEngine::PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}
}
