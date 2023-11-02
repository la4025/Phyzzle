#pragma once

#include <DirectXMath.h>

class ZeldaLight
{
public:
	ZeldaLight();
	ZeldaLight(const ZeldaLight& zeldaLight);
	~ZeldaLight();

	void SetAmbient(float red, float green, float blue, float alpha);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetSpecular(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

	DirectX::XMFLOAT4 GetAmbient();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT4 GetSpecular();
	DirectX::XMFLOAT3 GetDirection();

private:
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;

};

