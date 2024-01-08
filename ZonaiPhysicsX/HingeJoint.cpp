#include "HingeJoint.h"


namespace ZonaiPhysics
{
	HingeJoint::HingeJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{

	}

	HingeJoint::~HingeJoint() noexcept
	{
		joint->release();
	}

	float HingeJoint::GetAngle() const
	{
		return joint->getAngle();
	}

	float HingeJoint::GetVelocity() const
	{
		return joint->getVelocity();
	}

	void HingeJoint::SetLimit(float _upper, float _lower)
	{
		joint->setLimit({ _lower, _upper });
	}

	void HingeJoint::SetLimit(float _upper, float _lower, float _stiffness, float _damping)
	{
		joint->setLimit({ _lower, _upper, {_stiffness, _damping} });
	}
}
