#pragma once

#include "Light.h"
#include <Eigen/Dense>

namespace PurahEngine
{
	class PURAHENGINE_API DirectionalLight final : public Light
	{
	public:
		void Initialize() override;
		~DirectionalLight();

	};
}

