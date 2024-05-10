#pragma once
#include <Eigen/Dense>

#include "ZnPoint.h"

namespace ZonaiPhysics
{
	struct ZnTriangleMesh
	{
		std::vector<ZnPoint>	points;
		std::vector<int>		indies;
	};
}
