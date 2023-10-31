#pragma once
#include "IRigidBody.h"

namespace ZonaiMath
{
	class Vector3D;
}


namespace ZonaiPhysics
{
	class RigidBody : public IRigidBody
	{
	public:
		virtual bool Collide() noexcept;
		virtual ZonaiMath::Vector3D getLinerVelocity() const noexcept;
	};
}

