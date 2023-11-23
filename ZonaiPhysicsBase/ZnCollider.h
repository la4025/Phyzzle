#pragma once
#include "ZnObject.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}


namespace ZonaiPhysics
{
	using namespace ZonaiMath;

	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = 0;

	public:
		virtual Vector3D	GetLocalPosition() const noexcept = 0;
		virtual void		SetLocalPosition(const Vector3D& _position) noexcept  = 0;

		virtual Quaternion	GetLocalQuaternion() const noexcept = 0;
		virtual void		SetLocalQuaternion(const Quaternion& _quaternion) noexcept = 0;
	};
}