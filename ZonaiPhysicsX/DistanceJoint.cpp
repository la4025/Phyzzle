#include "DistanceJoint.h"

namespace ZonaiPhysics
{

	DistanceJoint::DistanceJoint(physx::PxPhysics*& _factory, RigidBody* _object0, const ZnTransform& _transform0, RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		joint->
	}

	DistanceJoint::~DistanceJoint() noexcept
	{

	}

	void DistanceJoint::SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept
	{

	}

	Eigen::Vector3f DistanceJoint::GetLocalPosition(eOBJECT) const noexcept
	{

	}

	void DistanceJoint::SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf&) noexcept
	{

	}

	Eigen::Quaternionf DistanceJoint::GetLocalQuaternion(eOBJECT) const noexcept
	{

	}

	Eigen::Vector3f DistanceJoint::GetRelativeLinearVelocity() const noexcept
	{

	}

	Eigen::Vector3f DistanceJoint::GetRelativeAngularVelocity() const noexcept
	{

	}

	void DistanceJoint::SetBreakForce(float _force, float _torque) noexcept
	{

	}

	void DistanceJoint::GetBreakForce(float& _force, float& _torque) const noexcept
	{

	}

	float DistanceJoint::GetDistance() const
	{
		return joint->getDistance();
	}

	void DistanceJoint::SetMinDistance(float _distance)
	{
		joint->setMinDistance(_distance);
	}

	float DistanceJoint::GetMinDistance() const
	{
		return joint->getMinDistance();
	}

	void DistanceJoint::SetMaxDistance(float _distance)
	{
		joint->setMaxDistance(_distance);
	}

	float DistanceJoint::GetMaxDistance() const
	{
		return joint->getMaxDistance();
	}

	void DistanceJoint::SetTolerance(float _tolerance)
	{
		joint->setTolerance(_tolerance);
	}

	float DistanceJoint::GetTolerance() const
	{
		return joint->getTolerance();
	}

	void DistanceJoint::SetStiffness(float _stiffness)
	{
		joint->setStiffness(_stiffness);
	}

	float DistanceJoint::GetStiffness() const
	{
		return joint->getStiffness();
	}

	void DistanceJoint::SetDanmping(float _damping)
	{
		joint->setDamping(_damping);
	}

	float DistanceJoint::GetDanmping() const
	{
		return joint->getDamping();
	}

	void DistanceJoint::SetMinDistanceEnable(bool _value)
	{
		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, _value);
	}

	void DistanceJoint::SetMaxDistanceEnable(bool _value)
	{
		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, _value);
	}

	void DistanceJoint::SetSpringEnable(bool _value)
	{
		using namespace physx;
		joint->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, _value);
	}
}