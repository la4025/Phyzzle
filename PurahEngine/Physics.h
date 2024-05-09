#pragma once
#include "PurahEngineAPI.h"
#include <Eigen/Dense>

#include "MaterialEnum.h"
#include "ZnRaycastInfo.h"

namespace ZonaiPhysics
{
	struct ZnRaycastInfo;
}

namespace PurahEngine
{
	class PURAHENGINE_API Physics
	{
	public:
		static ZonaiPhysics::ZnMaterialID AddMaterial(
			float _staticFriction, float _dynamicFriction, float _restitution,
			int _combineFriction, int _combineRestitution);

		static bool Raycast(const Eigen::Vector3f& _from, const Eigen::Vector3f& _to, float _distace, ZonaiPhysics::ZnRaycastInfo& _info);
	};
}
