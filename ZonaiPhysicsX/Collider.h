#pragma once
#include "ZnObject.h"

namespace physx
{
	class PxPhysics;
	class PxRigidDynamic;
	class PxRigidActor;
}

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	class Collider
	{
	public:
		Collider() noexcept = delete;
		Collider(physx::PxPhysics*&) noexcept;
		virtual				~Collider() noexcept;
	};
} // namespace ZonaiPhysics

