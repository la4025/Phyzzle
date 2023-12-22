#pragma once
#include <Eigen/Dense>
#include "ZnJoint.h"

namespace physx
{
	class PxJoint;
}

namespace ZonaiPhysics
{
	class ZnRigidBody;
	class ZnObject;

	class Joint : public ZnJoint
	{
	public:
					Joint() noexcept;
					~Joint() noexcept override;

	public:
		//void		SetObject(ZnObject*, ZnObject*) noexcept override;
		//void		GetObject(ZnObject*&, ZnObject*&) const noexcept override;

		void		SetLocalPosition(eOBJECT, const Eigen::Vector3f&) noexcept override;
		Eigen::Vector3f	GetLocalPosition(eOBJECT) const noexcept override;

		void		SetLocalQuaternion(eOBJECT, const Eigen::Quaternionf &) noexcept override;
		Eigen::Quaternionf	GetLocalQuaternion(eOBJECT) const noexcept override;

		Eigen::Vector3f	GetRelativeLinearVelocity() const noexcept override;
		Eigen::Vector3f	GetRelativeAngularVelocity() const noexcept override;

		void		SetBreakForce(float _force, float _torque) noexcept override;
		void		GetBreakForce(float& _force, float& _torque) const noexcept override;

	protected:
		ZnRigidBody* object[2];
		physx::PxJoint* joint;
	};
} // namespace ZonaiPhysics

