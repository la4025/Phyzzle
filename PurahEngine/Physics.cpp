#include "PhysicsSystem.h"

#include "Physics.h"

#include "ZnQueryDesc.h"

namespace PurahEngine
{
	bool Physics::Raycast(
		const Eigen::Vector3f& _from, 
		const Eigen::Vector3f& _to, float _distance,
		const std::vector<int>& _layers,
		ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _from;
		desc.direction = _to;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Raycast(desc, _info);
	}

	bool Physics::Boxcast(const Eigen::Vector3f& _extend, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Boxcast(_extend, desc, _info);
	}

	bool Physics::Spherecast(float radius, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Spherecast(radius, desc, _info);
	}

	bool Physics::Capsulecast(float height, float radius, const Eigen::Vector3f& _pos, const Eigen::Quaternionf& _rot,
		const Eigen::Vector3f& _dir, float _distance, const std::vector<int>& _layers, ZonaiPhysics::ZnQueryInfo& _info)
	{
		ZonaiPhysics::ZnQueryDesc desc;
		desc.position = _pos;
		desc.rotation = _rot;
		desc.direction = _dir;
		desc.distance = _distance;
		desc.queryLayer = _layers;

		return PhysicsSystem::GetInstance().Capsulecast(radius, height, desc, _info);
	}
}
