#pragma once
#include "ZonaiMath.h"

namespace ZonaiMath
{
	class Vector3D;
	class Quaternion;
}

namespace ZonaiPhysics
{
	using namespace ZonaiMath;

	class ZnTransform
	{
	public:
		Vector3D p;
		Quaternion q;
	};
}