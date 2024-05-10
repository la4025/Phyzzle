#include "PhysicsSystem.h"

#include "Physics.h"

namespace PurahEngine
{
	bool Physics::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distace, ZonaiPhysics::ZnRaycastInfo& _info)
	{
		return PhysicsSystem::GetInstance().Raycast(_from, _to, _distace, _info);
	}
}
