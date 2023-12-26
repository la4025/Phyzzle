#pragma once
#include <Eigen/Dense>
#include "Joint.h"

namespace physx
{
	class PxFixedJoint;
}

namespace ZonaiPhysics
{
	struct ZnTransform;

	class FixedJoint : public Joint
	{
	public:
							FixedJoint() noexcept = default;
							FixedJoint(physx::PxPhysics*& _factory, RigidBody* _object0, const ZnTransform& _transform0, RigidBody* _object1, const ZnTransform& _transform1) noexcept;
		virtual				~FixedJoint() noexcept = default;

	public:
		//virtual void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		//virtual Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		//virtual void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf &) noexcept override;
		//virtual Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		//virtual Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		//virtual Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		//virtual void		SetBreakForce(float _force, float _torque) noexcept override;
		//virtual void		GetBreakForce(float& _force, float& _torque) const noexcept override;

	private:
		physx::PxFixedJoint* joint;
	};
} // namespace ZonaiPhysics

