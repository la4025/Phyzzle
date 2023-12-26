#include "PxPhysicsAPI.h"

#include "RigidBody.h"
#include "FixedJoint.h"

#include "ZnTransform.h"

namespace ZonaiPhysics
{
	FixedJoint::FixedJoint(
		physx::PxPhysics*& _factory,
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1
	) noexcept : joint(nullptr)
	{
		using namespace physx;

		PxTransform t0, t1;

		t0.p.x = _transform0.position.x();
		t0.p.y = _transform0.position.y();
		t0.p.z = _transform0.position.z();

		t0.q.x = _transform0.quaternion.x();
		t0.q.y = _transform0.quaternion.y();
		t0.q.z = _transform0.quaternion.z();
		t0.q.w = _transform0.quaternion.w();

		t1.p.x = _transform1.position.x();
		t1.p.y = _transform1.position.y();
		t1.p.z = _transform1.position.z();

		t1.q.x = _transform1.quaternion.x();
		t1.q.y = _transform1.quaternion.y();
		t1.q.z = _transform1.quaternion.z();
		t1.q.w = _transform1.quaternion.w();

		joint = physx::PxFixedJointCreate(
			*_factory, 
			_object0->getRigidDynamic(), t0, 
			_object1->getRigidDynamic(), t1
		);
	}

	//void FixedJoint::SetObject(ZnObject* zn_object, ZnObject* zn_object1) noexcept
	//{
	//	Joint::SetObject(zn_object, zn_object1);
	//}

	//void FixedJoint::GetObject(ZnObject*&, ZnObject*&) const noexcept
	//{

	//}

	/*void FixedJoint::SetLocalPosition(eOBJECT _index, const Eigen::Vector3f& _p) noexcept
	{
		using namespace physx;
		PxTransform transform = joint->getLocalPose(static_cast<PxJointActorIndex::Enum>(_index));
		transform.p.x = _p.x;
		transform.p.y = _p.y;
		transform.p.z = _p.z;
		joint->setLocalPose(static_cast<PxJointActorIndex::Enum>(_index), transform);
	}

	Eigen::Vector3f FixedJoint::GetLocalPosition(eOBJECT _index) const noexcept
	{
		using namespace physx;
		PxTransform transform = joint->getLocalPose(static_cast<PxJointActorIndex::Enum>(_index));
		return {transform.p.x, transform.p.y, transform.p.z};
	}

	void FixedJoint::SetLocalQuaternion(eOBJECT _index, const Eigen::Quaternionf & _q) noexcept
	{
		using namespace physx;
		PxTransform transform = joint->getLocalPose(static_cast<PxJointActorIndex::Enum>(_index));
		transform.q.w = _q.w;
		transform.q.x = _q.x;
		transform.q.y = _q.y;
		transform.q.z = _q.z;
		joint->setLocalPose(static_cast<PxJointActorIndex::Enum>(_index), transform);
	}

	Eigen::Quaternionf FixedJoint::GetLocalQuaternion(eOBJECT _index) const noexcept
	{
		using namespace physx;
		PxTransform transform = joint->getLocalPose(static_cast<PxJointActorIndex::Enum>(_index));
		return {transform.q.w, transform.q.x, transform.q.y, transform.q.z};
	}

	Eigen::Vector3f FixedJoint::GetRelativeLinearVelocity() const noexcept
	{
		using namespace physx;
		PxVec3 velo = joint->getRelativeLinearVelocity();
		return { velo.x, velo.y, velo.z };
	}

	Eigen::Vector3f FixedJoint::GetRelativeAngularVelocity() const noexcept
	{
		using namespace physx;
		PxVec3 velo = joint->getRelativeAngularVelocity();
		return {velo.x, velo.y, velo.z};
	}*/

	/*void FixedJoint::SetBreakForce(float _force, float _torque) noexcept
	{
		joint->setBreakForce(_force, _torque);
	}

	void FixedJoint::GetBreakForce(float& _force, float& _torque) const noexcept
	{
		joint->getBreakForce(_force, _torque);
	}*/

	//void FixedJoint::SetLocalPosition(const Eigen::Vector3f& _localPosition) noexcept
	//{
	//	// joint->setLocalPose()
	//}

	//Eigen::Vector3f FixedJoint::GetLocalPosition() const noexcept
	//{
	//	return {};
	//}

	//void FixedJoint::SetLocalQuaternion(const Eigen::Quaternionf &) noexcept
	//{
	//	//joint->setLocalPose();
	//}

	//Eigen::Quaternionf FixedJoint::GetLocalQuaternion() const noexcept
	//{
	//	return {};
	//}
}
