#include "ZnSimulationCallbackX.h"

#include <PxConstraint.h>
#include <PxShape.h>

#include "ZnCollider.h"
#include "ZnCollision.h"
#include "ZnRigidBody.h"


namespace ZonaiPhysics
{
	// 웨이크 콜백
	void ZnSimulationCallbackX::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
			callback->OnWake(static_cast<ZnRigidBody*>(actors[i]->userData));
	}

	// 슬리프 콜백
	void ZnSimulationCallbackX::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		for (physx::PxU32 i = 0; i < count; i++)
			callback->OnSleep(static_cast<ZnRigidBody*>(actors[i]->userData));
	}

	// 트리거 콜백
	void ZnSimulationCallbackX::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		using namespace physx;

		for (physx::PxU32 i = 0; i < count; i++)
		{
			const PxTriggerPair& tp = pairs[i];

			if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			const auto trigger = GetCollider(tp.triggerShape);
			const auto other = GetCollider(tp.otherShape);

			assert(trigger != nullptr && other != nullptr);

			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				callback->OnTriggerEnter(trigger, other);

			else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
				callback->OnTriggerExit(trigger, other);

			else
				assert(false);
		}
	}

	// 제약 파괴 콜백
	void ZnSimulationCallbackX::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		// 파괴된 제약의 정보를 넘겨줌
		for (physx::PxU32 i = 0; i < count; i++)
		{
			callback->OnConstraintBreak(static_cast<ZnJoint*>(constraints[i].constraint->userData));
		}
	}

	// 충돌 콜벡
	// Header : 
	// Pair	  : 
	// Pairs  : 
	void ZnSimulationCallbackX::onContact(const physx::PxContactPairHeader& pairHeader,
		const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		using namespace physx;

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			const PxContactPair& cp = pairs[i];

			PxContactStreamIterator itr(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount, cp.contactCount);


			PxF32 buffer[3];
			cp.extractContacts(reinterpret_cast<PxContactPairPoint*>(buffer), cp.contactCount);
			const auto* collision = reinterpret_cast<ZnCollision*>(buffer);

			const auto trigger = GetCollider(cp.shapes[0]);
			const auto other = GetCollider(cp.shapes[1]);

			// PxContactPairPoint buffer;
			// cp.extractContacts(&buffer, cp.contactCount);
			// const auto* collision = reinterpret_cast<ZnCollision*>(&buffer);

			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				callback->OnCollisionEnter(trigger, other, *collision);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
				callback->OnCollisionStay(trigger, other, *collision);

			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				callback->OnCollisionExit(trigger, other, *collision);
			else
				assert(false);
		}
	}

	void ZnSimulationCallbackX::onAdvance(const physx::PxRigidBody* const* bodyBuffer,
		const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		// 구현 안함.
	}

	ZnCollider* ZnSimulationCallbackX::GetCollider(physx::PxShape* _shape)
	{
		return static_cast<ZnCollider*>(_shape->userData);
	}
}
