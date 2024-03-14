#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "ColliderHelper.h"

#include "CapsuleCollider.h"

namespace ZonaiPhysics
{
	CapsuleCollider::CapsuleCollider(physx::PxShape* _pxShape, RigidBody* _znBody) :
		Collider(_pxShape, _znBody)
	{

	}

	CapsuleCollider::~CapsuleCollider()
	{

	}
}
