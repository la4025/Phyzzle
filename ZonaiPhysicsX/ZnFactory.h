#pragma once
#include "ZnFactory.h"

#include "ZnMaterial.h"
#include "ZnTransform.h"

namespace ZonaiPhysics
{
	class Joint;
	class Collider;
	class RigidBody;

	class FactoryX
	{
	public:
		RigidBody* CreateRigidBody(void* _userData);
		Collider* CreateCollider(void* _userData, bool _trigger = false, const ZnMaterial* _material = nullptr);
		Joint* CreateJoint(void* _userData0, void* _userData1, const ZnTransform&, const ZnTransform&);
	};
}
