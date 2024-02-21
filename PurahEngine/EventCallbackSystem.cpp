#include "EventCallbackSystem.h"

#include "GameObject.h"
#include "ZnCollider.h"

namespace PurahEngine
{
	class Collider;

	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnConstraintBreak(const ZonaiPhysics::ZnJoint*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 =  _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<Collider*>(user1);

		g0->OnTriggerEnter(g0);
		g1->OnTriggerEnter(g1);
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<Collider*>(user1);

		// g0->OnTriggerExit(user1);
		// g1->OnTriggerExit(user0);
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<Collider*>(user1);

		// g0->OnCollisionExit(user1, _collision);
		// g1->OnCollisionExit(user0, _collision);
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<Collider*>(user1);

		// g0->OnCollisionExit(user1, _collision);
		// g1->OnCollisionExit(user0, _collision);
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto g0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto g1 = static_cast<Collider*>(user1);

		// g0->OnCollisionExit(user1, _collision);
		// g1->OnCollisionExit(user0, _collision);
	}
}
