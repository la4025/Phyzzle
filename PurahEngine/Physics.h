#pragma once
#include <Eigen/Dense>

#include "MaterialEnum.h"

namespace PurahEngine
{
	class Physics
	{
		static void AddMaterial(int, float, float, float, ZonaiPhysics::eCombineMode = ZonaiPhysics::eAVERAGE, ZonaiPhysics::eCombineMode = ZonaiPhysics::eAVERAGE);

		static bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distace, ZonaiPhysics::ZnRaycastInfo& _info);
	};
}
