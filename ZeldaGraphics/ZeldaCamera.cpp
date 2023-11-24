#include "ZeldaCamera.h"

#include "ResourceManager.h"

using namespace DirectX;

const float ZeldaCamera::DEFUALT_FOV = 3.141592654f / 4.0f;
const float ZeldaCamera::DEFUALT_NEAR = 1.0f;
const float ZeldaCamera::DEFUALT_FAR = 1000.0f;
CameraID ZeldaCamera::mainCameraID = CameraID::ID_NULL;

ZeldaCamera::ZeldaCamera(float screenWidth, float screenHeight) :
	fieldOfView(DEFUALT_FOV),
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	cameraNear(DEFUALT_NEAR),
	cameraFar(DEFUALT_FAR),
	transformMatrix(XMMatrixIdentity()),
	viewMatrix(XMMatrixIdentity()),
	projMatrix(XMMatrixIdentity()),
	orthoMatrix(XMMatrixIdentity()),
	isUpdatedViewMatrix(false),
	isUpdatedProjMatrix(false),
	isUpdatedOrthoMatrix(false)
{

}

ZeldaCamera::~ZeldaCamera()
{

}

void ZeldaCamera::SetTransformMatrix(DirectX::XMMATRIX matrix)
{
	isUpdatedViewMatrix = false;

	transformMatrix = matrix;
}

void ZeldaCamera::SetOption(float fieldOfView, float cameraNear, float cameraFar)
{
	isUpdatedProjMatrix = false;
	isUpdatedOrthoMatrix = false;

	this->fieldOfView = fieldOfView;
	this->cameraNear = cameraNear;
	this->cameraFar = cameraFar;
}

void ZeldaCamera::SetScreenSize(float screenWidth, float screenHeight)
{
	isUpdatedOrthoMatrix = false;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

DirectX::XMMATRIX ZeldaCamera::GetViewMatrix()
{
	if (isUpdatedViewMatrix)
	{
		return viewMatrix;
	}

	isUpdatedViewMatrix = true;

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	XMVECTOR scaleVector;
	XMVECTOR positionVector;
	XMVECTOR rotationQuater;
	XMMATRIX rotationMatrix;

	XMMatrixDecompose(&scaleVector, &rotationQuater, &positionVector, transformMatrix);

	rotationMatrix = XMMatrixRotationQuaternion(rotationQuater);

	XMVECTOR transformedLookAt = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Calculate the view matrix.
	viewMatrix = XMMatrixLookToLH(positionVector, transformedLookAt, upVector);

	return viewMatrix;
}

DirectX::XMMATRIX ZeldaCamera::GetProjMatrix()
{
	if (isUpdatedProjMatrix)
	{
		return projMatrix;
	}

	isUpdatedProjMatrix = true;

	float screenAspect = screenWidth / screenHeight;
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, cameraNear, cameraFar);

	return projMatrix;
}

DirectX::XMMATRIX ZeldaCamera::GetOrthoMatrix()
{
	if (isUpdatedOrthoMatrix)
	{
		return orthoMatrix;
	}

	isUpdatedOrthoMatrix = true;

	orthoMatrix = DirectX::XMMatrixOrthographicLH(screenWidth, screenHeight, cameraNear, cameraFar);

	return orthoMatrix;
}

bool ZeldaCamera::SetMainCamera(CameraID cameraID)
{
	if (ResourceManager::GetInstance().CheckCameraID(cameraID))
	{
		mainCameraID = cameraID;

		return true;
	}

	return false;
}

CameraID ZeldaCamera::GetMainCamera()
{
	return mainCameraID;
}
