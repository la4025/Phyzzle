#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "HingeJoint.h"

namespace PurahEngine
{
	HingeJoint::~HingeJoint()
	{
	}

	void HingeJoint::Initialize()
	{
		JointT<ZonaiPhysics::ZnHingeJoint>::Initialize();
	}

	void HingeJoint::OnDataLoadComplete()
	{
		const auto& instance = PhysicsSystem::GetInstance();

		const RigidBody* body0 = GetGameObject()->GetComponent<RigidBody>();

		assert(body0 != nullptr);

		joint = instance.CreateHingeJoint(
			body0->body, { LocalAnchor, LocalAnchorRotation },
			connectedBody->body, { connectedLocalAnchor, connectedLocalAnchorRotation }
		);

		if (useSpring)
		{
		}
		SetSpringArg(spring, damper);

		SetLimitEnable(useLimit);
		SetLimit(angleLower, angleUpper);

		SetDriveEnable(setMotor);
		SetDriveVelocity(velocity);
		SetDriveFreespin(freeSpin);



		JointT::OnDataLoadComplete();
	}

	float HingeJoint::GetAngle() const
	{
		return joint->GetAngle();
	}

	float HingeJoint::GetVelocity() const
	{
		return joint->GetVelocity();
	}

	void HingeJoint::SetLimit(float _lower, float _upper) const
	{
		joint->SetLimit(_lower, _upper);
	}

	void HingeJoint::GetLimit(float& _upper, float& _lower) const
	{
		joint->GetLimit(_upper, _lower);
	}

	float HingeJoint::GetRestitution() const
	{
		return joint->GetRestitution();
	}

	void HingeJoint::SetRestitution(float _restitution) const
	{
		joint->SetRestitution(_restitution);
	}

	float HingeJoint::GetBounceThreshold() const
	{
		return joint->GetBounceThreshold();
	}

	void HingeJoint::SetBounceThreshold(float _bounceThreshold) const
	{
		joint->SetBounceThreshold(_bounceThreshold);
	}

	void HingeJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void HingeJoint::GetSpringArg(float& _stiffness, float& _damping) const
	{
		joint->GetSpringArg(_stiffness, _damping);
	}

	void HingeJoint::SetDriveVelocity(float _velocity) const
	{
		joint->SetDriveVelocity(_velocity);
	}

	float HingeJoint::GetDriveVelocity() const
	{
		return joint->GetDriveVelocity();
	}

	void HingeJoint::SetDriveForceLimit(float _forceLimit) const
	{
		joint->SetDriveForceLimit(_forceLimit);
	}

	float HingeJoint::GetDriveForceLimit() const
	{
		return joint->GetDriveForceLimit();
	}

	void HingeJoint::SetDriveGearRatio(float _gearRatio) const
	{
		joint->SetDriveGearRatio(_gearRatio);
	}

	float HingeJoint::GetDriveGearRatio() const
	{
		return joint->GetDriveGearRatio();
	}

	void HingeJoint::SetLimitEnable(bool _value) const
	{
		joint->SetLimitEnable(_value);
	}

	void HingeJoint::SetDriveEnable(bool _value) const
	{
		joint->SetDriveEnable(_value);
	}

	void HingeJoint::SetDriveFreespin(bool _value) const
	{
		joint->SetDriveFreespin(_value);
	}

	void HingeJoint::PreSerialize(json& jsonData) const
	{

	}

	void HingeJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);
		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damper);
		PREDESERIALIZE_VALUE(targetPosition);

		PREDESERIALIZE_VALUE(setMotor);
		PREDESERIALIZE_VALUE(velocity);
		PREDESERIALIZE_VALUE(force);
		PREDESERIALIZE_VALUE(freeSpin);

		PREDESERIALIZE_VALUE(useLimit);
		PREDESERIALIZE_VALUE(angleLower);
		PREDESERIALIZE_VALUE(angleUpper);
		PREDESERIALIZE_VALUE(bounciness);
		PREDESERIALIZE_VALUE(bounceThreshold);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void HingeJoint::PostSerialize(json& jsonData) const
	{

	}

	void HingeJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(connectedBody);
	}
}