#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class ZnObject;

	enum ObjectIndex
	{
		OBJECT0,
		OBJECT1,
	};

	class Joint : public ZnJoint
	{
	public:
						Joint() noexcept = default;
		virtual			~Joint() noexcept;

	public:
		virtual void		SetObject(ZnObject*, ZnObject*) noexcept = 0;
		virtual void		GetObject(ZnObject*&, ZnObject*&) const noexcept = 0;

		virtual void		SetLocalPosition(ObjectIndex, const Vector3D&) noexcept = 0;
		virtual Vector3D	GetLocalPosition(ObjectIndex) const noexcept = 0;

		virtual void		SetLocalQuaternion(ObjectIndex, const Quaternion&) noexcept = 0;
		virtual Quaternion	GetLocalQuaternion(ObjectIndex) const noexcept = 0;

		virtual Vector3D	GetRelativeLinearVelocity() const noexcept = 0;
		virtual Vector3D	GetRelativeAngularVelocity() const noexcept = 0;

		virtual void		SetBreakForce(float _force, float _torque) noexcept = 0;
		virtual void		GetBreakForce(float& _force, float& _torque) const noexcept = 0;
	};
} // namespace ZonaiPhysics

