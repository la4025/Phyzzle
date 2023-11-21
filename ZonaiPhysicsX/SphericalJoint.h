#pragma once
#include "Joint.h"

namespace physx
{
	class PxSphericalJoint;
}

namespace ZonaiPhysics
{
	class SphericalJoint : public Joint
	{
	public:
		SphericalJoint() noexcept;
		virtual ~SphericalJoint() noexcept = default;

	public:

	private:
		physx::PxSphericalJoint* joint;
	};
}

