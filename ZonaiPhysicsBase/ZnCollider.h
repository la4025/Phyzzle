#pragma once
#include "ZnObject.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}


namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = 0;

	public:
		virtual ZonaiMath::Vector3D	GetLocalPosition() const noexcept = 0;
		virtual void		SetLocalPosition(const ZonaiMath::Vector3D& _position) noexcept  = 0;

		virtual ZonaiMath::Quaternion	GetLocalQuaternion() const noexcept = 0;
		virtual void		SetLocalQuaternion(const ZonaiMath::Quaternion& _quaternion) noexcept = 0;

		virtual void		SetTrigger(bool) noexcept = 0;
	};
}