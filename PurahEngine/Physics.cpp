#include "PhysicsSystem.h"

#include "Physics.h"

#include "ZnQueryDesc.h"

namespace PurahEngine
{
	bool Physics::Raycast(
		const Eigen::Vector3f& _from, 
		const Eigen::Vector3f& _to, float _distace,
		const std::vector<int>& _layers,
		ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _from;
		desc.direction = _to;
		desc.distance = _distace;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Raycast(desc, _info);
	}
}
