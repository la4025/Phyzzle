#include "RigidBody.h"
#include "ZnTransform.h"

#include "SphericalJoint.h"

#include <windows.h>

namespace ZonaiPhysics
{
	SphericalJoint::SphericalJoint(
		physx::PxPhysics*& _factory, 
		RigidBody* _object0, const ZnTransform& _transform0,
		RigidBody* _object1, const ZnTransform& _transform1) noexcept
	{
		object[0] = _object0;
		object[1] = _object1;

		using namespace physx;

		physx::PxTransform t0{}, t1{};

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

		PxRigidDynamic* rigid0 = nullptr;
		PxRigidDynamic* rigid1 = nullptr;

		if (_object0)
		{
			rigid0 = _object0->getRigidDynamic();
		}
		if (_object1)
		{
			rigid1 = _object1->getRigidDynamic();
		}

		joint = physx::PxSphericalJointCreate(
			*_factory,
			rigid0, t0,
			rigid1, t1
		);

		if (joint)
		{
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
	}

	SphericalJoint::~SphericalJoint() noexcept
	{
		joint->release();
	}

	void SphericalJoint::SetLocalPosition(eOBJECT _index, const Eigen::Vector3f& _localPos) noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		t.p = { _localPos.x(), _localPos.y() , _localPos.z() };
		joint->setLocalPose(index, t);
	}

	Eigen::Vector3f SphericalJoint::GetLocalPosition(eOBJECT _index) const noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		return { t.p.x, t.p.y , t.p.z };
	}

	void SphericalJoint::SetLocalQuaternion(eOBJECT _index, const Eigen::Quaternionf& _localQuat) noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		t.q = { _localQuat.x(), _localQuat.y() , _localQuat.z(), _localQuat.w() };
		joint->setLocalPose(index, t);
	}

	Eigen::Quaternionf SphericalJoint::GetLocalQuaternion(eOBJECT _index) const noexcept
	{
		using namespace physx;
		const auto index = static_cast<PxJointActorIndex::Enum>(_index);
		PxTransform t = joint->getLocalPose(index);
		return { t.q.x, t.q.y , t.q.z, t.q.w };
	}

	Eigen::Vector3f SphericalJoint::GetRelativeLinearVelocity() const noexcept
	{
		using namespace physx;
		const auto& velo = joint->getRelativeLinearVelocity();
		return { velo.x, velo.y ,velo.z };
	}

	Eigen::Vector3f SphericalJoint::GetRelativeAngularVelocity() const noexcept
	{
		using namespace physx;
		const auto& velo = joint->getRelativeAngularVelocity();
		return { velo.x, velo.y ,velo.z };
	}

	void SphericalJoint::SetBreakForce(float _force, float _torque) noexcept
	{
		joint->setBreakForce(_force, _torque);
	}

	void SphericalJoint::GetBreakForce(float& _force, float& _torque) const noexcept
	{
		joint->getBreakForce(_force, _torque);
	}

	void SphericalJoint::GetLimitAngle(float* _outY, float* _outZ)
	{
		auto cone = joint->getLimitCone();

		if (_outY)
		{
			*_outY = cone.yAngle;
		}
		if (_outZ)
		{
			*_outZ = cone.zAngle;
		}
	}

	void SphericalJoint::LimitEnable(bool _value)
	{
		using namespace physx;
		joint->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, _value);
	}

	void SphericalJoint::SetLimitCone(float _yAngle, float _zAngle)
	{
		joint->setLimitCone({ _yAngle, _zAngle });
	}

	void SphericalJoint::SetLimitCone(float _yAngle, float _zAngle, float _stiffness, float _damping)
	{
		joint->setLimitCone({ _yAngle, _zAngle, {_stiffness, _damping} });
	}
}
