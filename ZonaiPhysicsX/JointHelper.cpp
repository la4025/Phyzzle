#include <extensions/PxJoint.h>

#include "JointHelper.h"

namespace ZonaiPhysics
{
	void JointHelper::Release(void* _joint)
	{
		assert(_joint != nullptr);

		static_cast<physx::PxJoint*>(_joint)->release();
	}
}
