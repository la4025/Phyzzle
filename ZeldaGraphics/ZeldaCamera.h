#pragma once

#include <DirectXMath.h>

class ZeldaCamera
{
public:
	ZeldaCamera();
	ZeldaCamera(const ZeldaCamera& zeldaCamera);
	~ZeldaCamera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);

private:
	float mPositionX, mPositionY, mPositionZ;
	float mRotationX, mRotationY, mRotationZ;
	DirectX::XMMATRIX mViewMatrix;
};

