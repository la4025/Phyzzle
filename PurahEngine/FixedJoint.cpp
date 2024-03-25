#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnTransform.h"
#include "RigidBody.h"
#include "ZnRigidBody.h"

#include "FixedJoint.h"

namespace PurahEngine
{
	FixedJoint::~FixedJoint()
	{
		auto& instance = PhysicsSystem::GetInstance();

		instance.FreeObject(joint, GetGameObject());
		instance.joints.erase(std::ranges::find(instance.joints, this));
	}

	void FixedJoint::OnDataLoadComplete()
	{
		auto& instance = PhysicsSystem::GetInstance();

		const RigidBody* body0 = GetGameObject()->GetComponent<RigidBody>();

		assert(body0 != nullptr);

		joint = instance.CreateFixedJoint(
			body0->body, { LocalAnchor, LocalAnchorRotation },
			connectedBody->body, { connectedLocalAnchor, connectedLocalAnchorRotation }
		);

		PhysicsSystem::GetInstance().joints.push_back(this);
		joint->SetUserData(this);

		JointT::OnDataLoadComplete();
	}

	void FixedJoint::PreSerialize(json& jsonData) const
	{

	}

	void FixedJoint::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		PREDESERIALIZE_VECTOR3F(LocalAnchor);
		PREDESERIALIZE_QUATERNIONF(LocalAnchorRotation);
		PREDESERIALIZE_VECTOR3F(connectedLocalAnchor);
		PREDESERIALIZE_QUATERNIONF(connectedLocalAnchorRotation);

		PREDESERIALIZE_VALUE(breakForce);
		PREDESERIALIZE_VALUE(breakTorque);
		PREDESERIALIZE_VALUE(enableCollision);
		PREDESERIALIZE_VALUE(enablePreprocessing);
	}

	void FixedJoint::PostSerialize(json& jsonData) const
	{

	}

	void FixedJoint::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(connectedBody);
	}
}
