#include "EventCallbackSystem.h"

namespace PurahEngine
{
	void EventCallbackSystem::OnWake(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
	}

	void EventCallbackSystem::OnSleep(const ZonaiPhysics::ZnRigidBody*)
	{
		// 구현 안 함
	}

	void EventCallbackSystem::OnConstraintBreak(const ZonaiPhysics::ZnJoint*)
	{
		// 구현 안 함
	}

	void EventCallbackSystem::OnTriggerEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnTriggerExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionEnter(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionStay(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}

	void EventCallbackSystem::OnCollisionExit(const ZonaiPhysics::ZnCollider*, const ZonaiPhysics::ZnCollider*,
		const ZonaiPhysics::ZnCollision&)
	{
		// 
	}
}
