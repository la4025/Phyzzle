#include "PhysicsSystem.h"

#include "Physics.h"

namespace PurahEngine
{
	// void Physics::AddMaterial(
	// 	int _id, 
	// 	float _staticFriction, float _dynamicFriction, float _restitution,
	// 	ZonaiPhysics::eCombineMode _friction, ZonaiPhysics::eCombineMode _eRestitution)
	// {
	// 	return PhysicsSystem::GetInstance().AddMaterial(
	// 		_id, 
	// 		_staticFriction, 
	// 		_dynamicFriction,
	// 		_restitution,
	// 		_friction,
	// 		_eRestitution
	// 	);
	// }

	bool Physics::Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distace, ZonaiPhysics::ZnRaycastInfo& _info)
	{
		return PhysicsSystem::GetInstance().Raycast(_from, _to, _distace, _info);
	}
}
