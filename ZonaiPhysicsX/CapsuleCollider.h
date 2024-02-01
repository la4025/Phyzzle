#pragma once
#include "Collider.h"

namespace physx
{
	class PxShape;
}

namespace ZonaiPhysics
{
	class RigidBody;

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() noexcept = delete;
		CapsuleCollider(physx::PxShape*, RigidBody*) noexcept;
		~CapsuleCollider() noexcept override;
	};
} // ZonaiPhysics

