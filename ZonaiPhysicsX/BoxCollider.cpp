#pragma warning(push)
#pragma warning(disable: 33010 26495 4819)
#include "PxPhysicsAPI.h"
#include <Eigen/Dense>
#pragma warning (pop)

#include "RigidBody.h"

#include "BoxCollider.h"

#include "ColliderHelper.h"

namespace ZonaiPhysics
{
	BoxCollider::BoxCollider(physx::PxShape* _pxShape, RigidBody* _znBody):
		Collider(_pxShape, _znBody)
	{
		const physx::PxGeometry& geom = pxShape->getGeometry();
		physx::PxTransform pose = _znBody->pxBody->getGlobalPose();
		physx::PxBounds3 aabb;
		physx::PxGeometryQuery::computeGeomBounds(aabb, geom, pose, 0.1f);
	}

	BoxCollider::~BoxCollider()
	{

	}
}
