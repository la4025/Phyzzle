#include "PrismaticJoint.h"

namespace ZonaiPhysics
{
	PrismaticJoint::PrismaticJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{

	}

	PrismaticJoint::~PrismaticJoint() noexcept
	{
		joint->release();
	}
}
