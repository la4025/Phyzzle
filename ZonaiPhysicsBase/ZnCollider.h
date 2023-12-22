#pragma once
#include "ZnObject.h"

namespace Eigen
{
	class Vector3f;
	class Quaternionf;
}

namespace ZonaiPhysics
{
	class ZnCollider : public ZnObject
	{
	public:
							ZnCollider() noexcept = default;
		virtual				~ZnCollider() noexcept = default;

	public:
		virtual void		SetTrigger(bool) noexcept = 0;
	};
}
