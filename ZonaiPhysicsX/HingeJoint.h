#pragma once
#include "ZnHingeJoint.h"
#include "TemplateJoint.h"
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace physx
{
	class PxPhysics;
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class HingeJoint : public TemplateJoint<ZnHingeJoint, physx::PxRevoluteJoint>
	{
	public:
		HingeJoint() noexcept = delete;
		HingeJoint(
			physx::PxPhysics*& _factory,
			RigidBody* _object0, const ZnTransform& _transform0,
			RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		~HingeJoint() noexcept override;

	public:
		float GetAngle() const;
		float GetVelocity() const;
		void SetLimit(float _upper, float _lower);
		void SetLimit(float _upper, float _lower, float _stiffness, float _damping);
	};
} // namespace ZonaiPhysics

