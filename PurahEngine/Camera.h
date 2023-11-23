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


		void SetCameraPosition();

	private:
		CameraID cameraID;
		Eigen::Matrix4f camera;
	};
}

