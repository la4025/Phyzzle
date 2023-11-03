#pragma once

#include <DirectXMath.h>

class ZeldaCamera
{
public:
	~ZeldaCamera();

	void SetTransformMatrix(DirectX::XMMATRIX matrix);
	void SetOption(float fieldOfView, float cameraNear, float cameraFar);
	void SetScreenSize(float screenWidth, float screenHeight);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();

private:
	ZeldaCamera(float screenWidth, float screenHeight);
	ZeldaCamera(const ZeldaCamera& zeldaCamera) = delete;

	DirectX::XMMATRIX transformMatrix;
	float fieldOfView;
	float cameraNear;
	float cameraFar;

	float screenWidth;
	float screenHeight;

	bool isUpdatedViewMatrix;
	bool isUpdatedProjMatrix;
	bool isUpdatedOrthoMatrix;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMMATRIX orthoMatrix;

	const static float DEFUALT_FOV;
	const static float DEFUALT_NEAR;
	const static float DEFUALT_FAR;

	friend class ResourceManager;
};

