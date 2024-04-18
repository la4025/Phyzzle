#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class PlayerCamera final : public PurahEngine::Component
	{
	public:
		~PlayerCamera() override;

	public:
		void Start() override;
		void Update() override;
		void LateUpdate() override;

	public:
		Eigen::Vector3f GetArmForward();
		Eigen::Vector3f GetArmRight();
		Eigen::Vector3f GetArmUp();
		Eigen::Vector3f GetCoreForward();
		Eigen::Vector3f GetCoreRight();
		Eigen::Vector3f GetCoreUp();

		void CameraArmRotate();
		void CameraArmTranslate();
		void CameraCoreRotate();
		void CameraCoreTranslate();

	private:
		PurahEngine::Transform* arm;
		PurahEngine::Transform* core;

	private:

	};
}
