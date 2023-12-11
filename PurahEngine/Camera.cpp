#include "Camera.h"

const float DEFAULT_NEAR = 1.0f;
const float DEFAULT_FAR = 1000.0f;
const float DEFAULT_FOV = 3.141592654f / 4.0f;

PurahEngine::Camera::Camera()
	: cameraID(renderer->CreateCamera()), renderer(PurahEngine::GraphicsManager::GetInstance().GetRenderer()), 
	cameraNear(DEFAULT_NEAR), cameraFar(DEFAULT_FAR), fieldOfView(DEFAULT_FOV)
{
	renderer->SetMainCamera(cameraID);
}

PurahEngine::Camera::~Camera()
{

}

CameraID PurahEngine::Camera::GetCameraID()
{
	return cameraID;
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

void PurahEngine::Camera::UpdateCamera(const Eigen::Matrix4f& worldMatrix, float fieldOfView, float cameraNear, float cameraFar)
{
	renderer->UpdateCamera(cameraID, worldMatrix, fieldOfView, cameraNear, cameraFar);
}
