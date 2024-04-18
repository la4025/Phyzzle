#pragma once
#include "PurahEngine.h"
#include "Player.h"

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
		PurahEngine::Camera* GetMainCamera();
		PurahEngine::Camera* GetCamera(const std::string& _cameraName);
		void ChangeCamera(
			const std::string& _cameraName,
			float _lerpTime = 0.f);

	public:
		void CameraLookAt(const Eigen::Vector3f& _focus);
		void CameraLookTo(const Eigen::Vector3f& _focus);
		void CameraArmRotate(const Eigen::Vector3f& _axis, float _angle);
		void CameraArmTranslate(const Eigen::Vector3f& _translate);
		void CameraCoreRotate(const Eigen::Vector3f& _axis, float _angle);
		void CameraCoreTranslate(const Eigen::Vector3f& _translate);

	private:
		PurahEngine::Transform* currCamera;
		PurahEngine::Transform* arm;
		PurahEngine::Transform* core;

	private:
		Eigen::Vector3f startPosition;
		Eigen::Quaternionf startRotation;
		std::function<Eigen::Vector3f(Eigen::Vector3f, Eigen::Vector3f, float)> lerp;
		std::function<Eigen::Quaternionf(Eigen::Quaternionf, Eigen::Quaternionf, float)> slerp;

		std::map<std::string, PurahEngine::Transform*> cameraList;
	};
}
