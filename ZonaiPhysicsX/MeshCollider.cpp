#include "PxPhysicsAPI.h"
#include "ColliderHelper.h"

#include "MeshCollider.h"

namespace ZonaiPhysics
{
	MeshCollider::MeshCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{
	}

	MeshCollider::~MeshCollider()
	{
	}
}
