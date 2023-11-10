#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class ZnObject;

	class Joint : public ZnJoint
	{
	public:
						Joint() noexcept = default;
		virtual			~Joint() noexcept;

	public:
		virtual void		SetObject(ZnObject*, ZnObject*) noexcept override;
		virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept override;

		virtual void		SetLocalPosition(const Vector3D&) noexcept override;
		virtual Vector3D	GetLocalPosition() const noexcept override;

		virtual void		SetLocalQuaternion(const Quaternion&) noexcept override;
		virtual Quaternion	GetLocalQuaternion() const noexcept override;

		virtual Vector3D	GetRelativeLinearVelocity() const noexcept override;
		virtual Vector3D	GetRelativeAngularVelocity() const noexcept override;

		virtual void		SetBreakForce(float _force, float _torque) noexcept override;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept override;
	};
} // namespace ZonaiPhysics

