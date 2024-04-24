#include "PlayerCamera.h"

namespace Phyzzle
{
	PlayerCamera::~PlayerCamera()
	{
	}

	void PlayerCamera::Start()
	{
		lerp =
			[this](const Eigen::Vector3f start, const Eigen::Vector3f end, float _t) -> Eigen::Vector3f
			{
				if (start == end)
					return start;

				return start + _t * (end - start);
			};

		slerp =
			[this](const Eigen::Quaternionf start, const Eigen::Quaternionf end, float _t) -> Eigen::Quaternionf
			{
				if (start == end)
					return start;

				return 	start.slerp(_t, end);
			};
	}

	void PlayerCamera::Update()
	{

	}

	void PlayerCamera::LateUpdate()
	{
		auto dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

	}

	PurahEngine::Camera* PlayerCamera::GetMainCamera()
	{
		return currCamera;
	}

	PurahEngine::Camera* PlayerCamera::GetCamera(const std::string& _cameraName)
	{
		return cameraList[_cameraName];
	}

	void PlayerCamera::ChangeCamera(
		const std::string& _cameraName,
		float _lerpTime)
	{
		if (currCamera)
		{
			PurahEngine::Transform* currTransform = currCamera->GetGameObject()->GetTransform();
			currTransform->GetWorldPosition();
			currTransform->GetWorldRotation();
		}

		auto cameraTransform = cameraList[_cameraName];
	}

	void PlayerCamera::CameraLookAt(const Eigen::Vector3f& _position)
	{

	}

	void PlayerCamera::CameraLookTo(const Eigen::Vector3f& _focus)
	{

	}
}
