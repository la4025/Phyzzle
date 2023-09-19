#include "ZonaiMath.h"

namespace ZonaiMath
{
	class Matrix4x4;
}

namespace ZonaiPhysics
{
	struct MassData
	{
		float Mass;
		float invMass;
		ZonaiMath::Matrix4x4 inertia;
		ZonaiMath::Matrix4x4 invInertia;
	};

	class RigidBody
	{
	public:
		
		MassData massData;


	};
}