#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
namespace PurahEngine
{
	class PURAHENGINE_API Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		float GetCameraNear();
		float GetCameraFar();
		float GetCameraFOV();
	};
}

