#pragma once
#include <windows.h>

#include "ZnJoint.h"

namespace ZonaiPhysics
{

	class JointHelper
	{
	public:
		static void Release(void*);

		static void SetLocalPosition(ZnJoint::eOBJECT _index, const Vector3f& _localPos);
		static Vector3f GetLocalPosition(ZnJoint::eOBJECT _index);

		static void SetLocalQuaternion(ZnJoint::eOBJECT _index, const Quaternionf& _localQuat);
		static Quaternionf GetLocalQuaternion(ZnJoint::eOBJECT _index);

		static Vector3f GetRelativeLinearVelocity();
		static Vector3f GetRelativeAngularVelocity();

		static void SetBreakForce(float _force, float _torque);

	};
}

