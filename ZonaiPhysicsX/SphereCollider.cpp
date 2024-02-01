#include "PxPhysicsAPI.h"
#include "RigidBody.h"
#include "SphereCollider.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	SphereCollider::SphereCollider(physx::PxShape* _pxShape, RigidBody* _znBody) noexcept :
		Collider(_pxShape, _znBody)
	{
		shape->userData = this;
	}

	SphereCollider::~SphereCollider() noexcept
	{
		ColliderHelper::Release(shape);
		shape = nullptr;
		rigidbody = nullptr;
	}
}
