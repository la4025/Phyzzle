#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "ColliderHelper.h"

#include "CapsuleCollider.h"

namespace ZonaiPhysics
{
	CapsuleCollider::CapsuleCollider(physx::PxShape* _pxShape, RigidBody* _znBody) noexcept :
		Collider(_pxShape, _znBody)
	{
		shape->userData = this;
	}

	CapsuleCollider::~CapsuleCollider() noexcept
	{
		ColliderHelper::Release(shape);
		shape = nullptr;
		rigidbody = nullptr;
	}
}
