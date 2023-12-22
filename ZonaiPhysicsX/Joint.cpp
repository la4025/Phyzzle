#include "ZnObject.h"

#include "Joint.h"

#include <extensions/PxJoint.h>

namespace ZonaiPhysics
{
	Joint::Joint() noexcept
	= default;

	Joint::~Joint() noexcept
	= default;

	//void Joint::SetObject(ZnObject* _object0, ZnObject* _object1) noexcept
	//{
	//	object[0] = dynamic_cast<ZnRigidBody*>(_object0);
	//	object[1] = dynamic_cast<ZnRigidBody*>(_object1);
	//}

	//void Joint::GetObject(ZnObject*& _object0, ZnObject*& _object1) const noexcept
	//{
	//	_object0 = object[0];
	//	_object1 = object[1];
	//}

	void Joint::SetLocalPosition(eOBJECT _index, const Eigen::Vector3f& _localPos) noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		t.p = { _localPos.x, _localPos.y , _localPos.z };
		joint->setLocalPose(index, t);
	}

	Eigen::Vector3f Joint::GetLocalPosition(eOBJECT _index) const noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		return { t.p.x, t.p.y , t.p.z };
	}

	void Joint::SetLocalQuaternion(eOBJECT _index, const Eigen::Quaternionf& _localQuat) noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		t.q = { _localQuat.x, _localQuat.y , _localQuat.z, _localQuat.w };
		joint->setLocalPose(index, t);
	}

	Eigen::Quaternionf Joint::GetLocalQuaternion(eOBJECT _index) const noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		return { t.q.x, t.q.y , t.q.z, t.q.w };
	}

	Eigen::Vector3f Joint::GetRelativeLinearVelocity() const noexcept
	{
		using namespace physx;
		const auto& velo = joint->getRelativeLinearVelocity();
		return { velo.x, velo.y ,velo.z };
	}

	Eigen::Vector3f Joint::GetRelativeAngularVelocity() const noexcept
	{
		using namespace physx;
		const auto& velo = joint->getRelativeAngularVelocity();
		return { velo.x, velo.y ,velo.z };
	}

	void Joint::SetBreakForce(float _force, float _torque) noexcept
	{
		joint->setBreakForce(_force, _torque);
	}

	void Joint::GetBreakForce(float& _force, float& _torque) const noexcept
	{
		joint->getBreakForce(_force, _torque);
	}
}
