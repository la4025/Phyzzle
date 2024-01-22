#include "QuaryCallback.h"

#include <PxShape.h>

namespace ZonaiPhysics
{
	physx::PxQueryHitType::Enum QuaryCallback::preFilter(
		const physx::PxFilterData& filterData,
		const physx::PxShape* shape, 
		const physx::PxRigidActor* actor, 
		physx::PxHitFlags& queryFlags)
	{
		// Early out to avoid crashing
		if (!shape)
			return physx::PxQueryHitType::eNONE;

		// Check mask
		const physx::PxFilterData shapeFilter = shape->getQueryFilterData();
		if ((filterData.word0 & shapeFilter.word0) == 0)
		{
			return physx::PxQueryHitType::eNONE;
		}

		// Check if skip triggers
		const bool hitTriggers = filterData.word2 != 0;
		if (!hitTriggers && shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
			return physx::PxQueryHitType::eNONE;

		const bool blockSingle = filterData.word1 != 0;

		return blockSingle ? physx::PxQueryHitType::eBLOCK : physx::PxQueryHitType::eTOUCH;
	}

	physx::PxQueryHitType::Enum QuaryCallback::postFilter(
		const physx::PxFilterData& filterData,
		const physx::PxQueryHit& hit, 
		const physx::PxShape* shape, 
		const physx::PxRigidActor* actor)
	{
		// Not used
		return physx::PxQueryHitType::eNONE;
	}
}
