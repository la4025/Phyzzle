#include "PhysicsSystem.h"
#include "../ZonaiPhysicsBase/ZnPhysicsBase.h"
#include <cassert>

#include "EventCallbackSystem.h"
#include "RigidBody.h"

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

		physics = createZonaiPhysics();

		if (physics == nullptr)
		{
			assert(0);
		}

		callbackSystem = new EventCallbackSystem;

		physics->Initialize(callbackSystem);
	}

	void PhysicsSystem::Simulation(float _dt) noexcept
	{
		physics->Simulation(_dt);
		//
	}

	void PhysicsSystem::SimulateResult()
	{
		for (auto& e : bodies)
		{
			e->SimulateResult();
		}
	}

	void PhysicsSystem::Finalize() noexcept
	{
		physics->Finalize();

		FreeLibrary(ZonaiPhysicsXDLL);
	}

	ZonaiPhysics::ZnRigidBody* PhysicsSystem::CreateRigidBody(void* _gameObject) const noexcept
	{
		return physics->CreateRigidBody(_gameObject);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateBoxCollider(void* _gameObject, float x, float y, float z) const noexcept
	{
		return physics->CreateBoxCollider(_gameObject, { x, y, z }, 0);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateSphereCollider(void* _gameObject, float radius) const noexcept
	{
		return physics->CreateSphereCollider(_gameObject, radius, 0);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(void* _gameObject, float radius, float height) const noexcept
	{
		return physics->CreateCapsuleCollider(_gameObject, radius, height, 0);
	}

	PurahEngine::PhysicsSystem& PurahEngine::PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreatPlaneCollider(const std::wstring& _id, float x, float y) noexcept
	//{
	//	//return physics->CreatPlaneCollider(_id, x, y);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreatSphereCollider(const std::wstring& _id, float radius) noexcept
	//{
	//	//return physics->CreatSphereCollider(_id, radius);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(const std::wstring& _id, float radius, float height) noexcept
	//{
	//	//return physics->CreateCapsuleCollider(_id, radius, height);
	//}

	//ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCustomCollider(const std::wstring& _id) noexcept
	//{
	//	//return physics->CreateCustomCollider(_id);
	//}

}
