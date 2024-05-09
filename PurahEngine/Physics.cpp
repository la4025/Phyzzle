#include "PhysicsSystem.h"

#include "Physics.h"

namespace PurahEngine
{
	ZonaiPhysics::ZnMaterialID Physics::AddMaterial(
		float _staticFriction, float _dynamicFriction, float _restitution,
		int _combineFriction, int _combineRestitution)
	{
		return PhysicsSystem::GetInstance().AddMaterial(
			_staticFriction, 
			_dynamicFriction,
			_restitution,
			static_cast<ZonaiPhysics::eCombineMode>(_combineFriction),
			static_cast<ZonaiPhysics::eCombineMode>(_combineRestitution)
		);
	}

	bool Physics::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distace, ZonaiPhysics::ZnRaycastInfo& _info)
	{
		return PhysicsSystem::GetInstance().Raycast(_from, _to, _distace, _info);
	}
}
