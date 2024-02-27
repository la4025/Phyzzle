#include "Camera.h"

#include "SceneManager.h"
#include "GraphicsResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

namespace PurahEngine
{
	const float Camera::DEFAULT_NEAR = 1.0f;
	const float Camera::DEFAULT_FAR = 1000.0f;
	const float Camera::DEFAULT_FOV = 3.141592654f / 4.0f;

	PurahEngine::Camera::Camera()
		: cameraID(CameraID::ID_NULL),
		cameraNear(DEFAULT_NEAR),
		cameraFar(DEFAULT_FAR),
		fieldOfView(DEFAULT_FOV)
	{
		cameraID = GraphicsManager::GetInstance().resourceManager->CreateCamera();
	}

	PurahEngine::Camera::~Camera()
	{
		GraphicsManager::GetInstance().resourceManager->ReleaseCamera(cameraID);
	}

	void PurahEngine::Camera::SetCameraNear(float cameraNear)
	{
		this->cameraNear = cameraNear;
	}

	void PurahEngine::Camera::SetCameraFar(float cameraFar)
	{
		this->cameraFar = cameraFar;
	}

	void PurahEngine::Camera::SetCameraFOV(float fov)
	{
		this->fieldOfView = fov;
	}

	float PurahEngine::Camera::GetCameraNear()
	{
		return cameraNear;
	}

	float PurahEngine::Camera::GetCameraFar()
	{
		return cameraFar;
	}

	float PurahEngine::Camera::GetCameraFOV()
	{
		return fieldOfView;
	}

	void PurahEngine::Camera::SetMainCamera()
	{
		SceneManager::GetInstance().SetMainCamera(this);
	}

	void PurahEngine::Camera::Render(IZeldaRenderer* renderer)
	{
		renderer->SetMainCamera(cameraID);

		Eigen::Quaternionf rotation = GetGameObject()->GetTransform()->GetWorldRotation();
		Eigen::Vector3f position = GetGameObject()->GetTransform()->GetWorldPosition();

		// 회전 행렬을 만듭니다.
		Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
		rotationMatrix.block<3, 3>(0, 0) = rotation.toRotationMatrix();

		// 위치 행렬을 만듭니다.
		Eigen::Matrix4f translationMatrix = Eigen::Matrix4f::Identity();
		translationMatrix.block<3, 1>(0, 3) = position;

		// 변환 행렬을 결합합니다.
		Eigen::Matrix4f worldTM = translationMatrix * rotationMatrix;

		Eigen::Translation3f(GetGameObject()->GetTransform()->GetWorldPosition());

		renderer->UpdateCamera(cameraID, worldTM, fieldOfView, cameraNear, cameraFar);
	}
}