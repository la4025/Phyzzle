#pragma once

#include <DirectXMath.h>

class ZeldaLight
{
public:
	ZeldaLight();
	ZeldaLight(const ZeldaLight& zeldaLight);
	~ZeldaLight();

	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT3 GetDirection();

private:
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 direction;

};

