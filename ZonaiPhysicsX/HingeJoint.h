#pragma once
#include "ZnHingeJoint.h"
#include "TemplateJoint.h"

#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include <Eigen/Dense>
#include "PxPhysicsAPI.h"
#pragma warning (pop)

namespace physx
{
	class PxPhysics;
	class PxRevoluteJoint;
}

namespace ZonaiPhysics
{
	class ZnTransform;
	class ZnRigidBody;
	class RigidBody;

	class HingeJoint : public TemplateJoint<ZnHingeJoint, physx::PxRevoluteJoint>
	{
	public:
		HingeJoint() = delete;
		HingeJoint(physx::PxRevoluteJoint*, RigidBody*, RigidBody*);
		//HingeJoint(
		//	physx::PxPhysics*& _factory,
		//	RigidBody* _object0, const ZnTransform& _transform0,
		//	RigidBody* _object1, const ZnTransform& _transform1);
		~HingeJoint() override;

	public:
		float GetAngle() const override;
		float GetVelocity() const override;

		void SetLimit(float _upper, float _lower) override;
		void SetLimit(float _upper, float _lower, float restitution = 0.f, float _bounceThreshold = 0.5f);
		void SetLimitWithSpring(float _upper, float _lower, float _stiffness, float _damping) override;

		float GetRestitution() const;
		void SetRestitution() const;
		float GetBounceThreshold() const;
		float SetBounceThreshold() const;

		void GetLimit(float& _upper, float& _lower) const;
		void GetLimit(float& _upper, float& _lower, float& _stiffness, float& _damping) const;

		void SetDriveVelocity(float _velocity) override;
		float GetDriveVelocity() const override;

		void SetDriveForceLimit(float _limit) override;
		float GetDriveForceLimit() const override;

		void SetDriveGearRatio(float _ratio) override;
		float GetDriveGearRatio() const override;

		void SetLimitEnable(bool) override;
		void SetDriveEnable(bool) override;
		void SetDriveFreespin(bool) override;
	};
} // namespace ZonaiPhysics
