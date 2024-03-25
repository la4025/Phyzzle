#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"
#include "ZnJoint.h"

#include "Joint.h"
#include "Collider.h"

#include "EventCallbackSystem.h"

namespace PurahEngine
{
	class Joint;
	class Collider;

	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody* _body)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
		return;
	}

	void EventCallbackSystem::OnConstraintBreak(ZonaiPhysics::ZnJoint* _joint)
	{
		void* joint = _joint->GetUserData();
		const auto pJoint = static_cast<PurahEngine::Joint*>(joint);

		pJoint->BreakCallback();
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		//purahCollider0->GetGameObject()->OnTriggerExit(purahCollider1);
		//purahCollider1->GetGameObject()->OnTriggerExit(purahCollider0);
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		//purahCollider0->GetGameObject()->OnTriggerExit(purahCollider1);
		//purahCollider1->GetGameObject()->OnTriggerExit(purahCollider0);
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		// purahCollider0->GetGameObject()->OnCollisionEnter(_collision, purahCollider1);
		// purahCollider1->GetGameObject()->OnCollisionEnter(_collision, purahCollider0);
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		purahCollider0->GetGameObject()->OnCollisionStay(_collision, purahCollider1);
		purahCollider1->GetGameObject()->OnCollisionStay(_collision, purahCollider0);
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider* _c0, const ZonaiPhysics::ZnCollider* _c1,
		const ZonaiPhysics::ZnCollision& _collision)
	{
		const auto user0 = _c0->GetUserData();
		assert(user0 != nullptr);
		const auto purahCollider0 = static_cast<Collider*>(user0);

		const auto user1 = _c1->GetUserData();
		assert(user1 != nullptr);
		const auto purahCollider1 = static_cast<Collider*>(user1);

		// purahCollider0->GetGameObject()->OnCollisionExit(_collision, purahCollider1);
		// purahCollider1->GetGameObject()->OnCollisionExit(_collision, purahCollider0);
	}
}
