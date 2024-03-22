#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "SlideJoint.h"

#include <numbers>


namespace PurahEngine
{
	SlideJoint::~SlideJoint()
	{
	}

	void SlideJoint::Initialize()
	{
	}

	void SlideJoint::OnDataLoadComplete()
	{
		const auto& instance = PhysicsSystem::GetInstance();

		const RigidBody* body0 = GetGameObject()->GetComponent<RigidBody>();

		assert(body0 != nullptr);

		joint = instance.CreateSlideJoint(
			body0->body, { LocalAnchor },
			connectedBody->body, { connectedLocalAnchor }
		);

		// pring
		{
			float setSpring = 0.f;
			float setDamper = 0.f;
			if (useSpring)
			{
				setSpring = spring;
				setDamper = damper;
			}
			SetSpringArg(setSpring, setDamper);
		}

		// limit
		{
			float setMinLimit = FLT_MAX / 3.f;
			float setMaxLimit = -FLT_MAX / 3.f;
			float bounce = 0.f;
			if (useMinDistance)
			{
				setMinLimit = minDistance;
			}
			if (useMaxDistance)
			{
				setMaxLimit = maxDistance;
			}
			SetLimit(setMinLimit, setMaxLimit);
			SetBounciness(bounciness);
		}

		JointT::OnDataLoadComplete();
	}

	float SlideJoint::GetPosition() const
	{
		return joint->GetPosition();
	}

	float SlideJoint::GetVelocity() const
	{
		return joint->GetVelocity();
	}

	void SlideJoint::SetLimit(float _lower, float _upper) const
	{
		joint->SetLimit(_lower, _upper);
	}

	void SlideJoint::SetSpringArg(float _stiffness, float _damping) const
	{
		joint->SetSpringArg(_stiffness, _damping);
	}

	void SlideJoint::SetBounciness(float _bounciness) const
	{
		joint->SetRestitution(_bounciness);
	}

	void SlideJoint::SetLimitEnable(bool _value) const
	{
		joint->SetLimitEnable(_value);
	}

	bool SlideJoint::IsLimitEnabled() const
	{
		return joint->IsLimitEnabled();
	}

	void SlideJoint::PreSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);

		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);

		PREDESERIALIZE_VALUE(useSpring);
		PREDESERIALIZE_VALUE(spring);
		PREDESERIALIZE_VALUE(damper);

		PREDESERIALIZE_VALUE(useMinDistance);
		PREDESERIALIZE_VALUE(minDistance);
		PREDESERIALIZE_VALUE(useMaxDistance);
		PREDESERIALIZE_VALUE(maxDistance);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void SlideJoint::PostSerialize(json& jsonData) const
	{

	}

	void SlideJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(connectedBody);
	}
}
