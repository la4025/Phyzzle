#pragma once
#include "PurahEngineAPI.h"
#include <Eigen/Dense>

#include "MaterialEnum.h"
#include "ZnQueryInfo.h"

namespace ZonaiPhysics
{
	struct ZnQueryInfo;
}

namespace PurahEngine
{
	class PURAHENGINE_API Physics
	{
	public:
		static bool Raycast(const Eigen::Vector3f& _from,
			const Eigen::Vector3f& _to, float _distace,
			const std::vector<int>& _layers,
			ZonaiPhysics::ZnQueryInfo& _info);
	};
}
