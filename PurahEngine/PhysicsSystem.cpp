#include "PhysicsSystem.h"
#include "../ZonaiPhysicsBase/ZnPhysicsBase.h"

namespace PurahEngine
{

	void PhysicsSystem::Initialize() noexcept
	{
		// physics = new ZonaiPhysics::ZnPhysicsBase();
	}

	void PhysicsSystem::Simulation(float _dt) noexcept
	{
		physics->Simulation(_dt);
	}

	void PhysicsSystem::Finalize() noexcept
	{

	}

	ZonaiPhysics::ZnRigidBody* PhysicsSystem::CreateRigidBody(const std::wstring& _id) noexcept
	{
		return physics->CreateRigidBody(_id);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreatBoxCollider(const std::wstring& _id, float x, float y, float z) noexcept
	{
		return physics->CreatBoxCollider(_id, x, y, z);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreatPlaneCollider(const std::wstring& _id, float x, float y) noexcept
	{
		return physics->CreatPlaneCollider(_id, x, y);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreatSphereCollider(const std::wstring& _id, float radius) noexcept
	{
		return physics->CreatSphereCollider(_id, radius);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCapsuleCollider(const std::wstring& _id, float radius, float height) noexcept
	{
		return physics->CreateCapsuleCollider(_id, radius, height);
	}

	ZonaiPhysics::ZnCollider* PhysicsSystem::CreateCustomCollider(const std::wstring& _id) noexcept
	{
		return physics->CreateCustomCollider(_id);
	}

}