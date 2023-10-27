#include "ZeldaLight.h"

using namespace DirectX;

ZeldaLight::ZeldaLight() :
	diffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
	direction({ 0.0f, 0.0f, 1.0f })
{

}

ZeldaLight::ZeldaLight(const ZeldaLight& zeldaLight) :
	diffuse(zeldaLight.diffuse),
	direction(zeldaLight.direction)
{

}

ZeldaLight::~ZeldaLight()
{

}

void ZeldaLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuse = XMFLOAT4(red, green, blue, alpha);
}

void ZeldaLight::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT4 ZeldaLight::GetDiffuseColor()
{
	return diffuse;
}

DirectX::XMFLOAT3 ZeldaLight::GetDirection()
{
	return direction;
}
