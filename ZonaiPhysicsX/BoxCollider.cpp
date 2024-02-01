#include <Eigen/Dense>
#include "RigidBody.h"
#include "PxPhysicsAPI.h"

#include "BoxCollider.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	BoxCollider::BoxCollider(physx::PxShape* _pxShape, RigidBody* _znBody) noexcept:
		Collider(_pxShape, _znBody)
	{
		shape->userData = this;
	}

	BoxCollider::~BoxCollider() noexcept
	{
		ColliderHelper::Release(shape);
		shape = nullptr;
		rigidbody = nullptr;
	}
}
