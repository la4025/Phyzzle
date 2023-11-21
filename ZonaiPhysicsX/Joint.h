#pragma once
#include "ZnJoint.h"

namespace ZonaiPhysics
{
	class ZnObject;

<<<<<<< HEAD
	enum ObjectIndex
	{
		OBJECT0,
		OBJECT1,
	};

=======
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
	class Joint : public ZnJoint
	{
	public:
						Joint() noexcept = default;
		virtual			~Joint() noexcept;

	public:
<<<<<<< HEAD
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
=======
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
>>>>>>> 49329f7c854b5a1bc6f7f78c07b81085dead6dbe
	};
} // namespace ZonaiPhysics

