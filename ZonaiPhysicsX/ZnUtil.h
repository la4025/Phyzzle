#pragma once
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	Eigen::Vector3f PhysxToEigen(const physx::PxVec3& _vec)
	{
		return { _vec.x, _vec.y ,_vec.z };
	}

	Eigen::Quaternionf PhysxToEigen(const physx::PxQuat& _quat)
	{
		return { _quat.w, _quat.x, _quat.y ,_quat.z };
	}
}
