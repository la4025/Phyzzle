#pragma once
#include "Component.h"
#include "IZeldaRenderer.h"
namespace PurahEngine
{
	class Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		CameraID cameraID = CameraID::ID_NULL;

		void SetCameraPosition();

	private:
		Eigen::Matrix4f camera;
	};
}

