#include "ZeldaLight.h"

using namespace DirectX;

ZeldaLight::ZeldaLight() :
	ambient({ 0.2f, 0.2f, 0.2f, 1.0f }),
	diffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
	specular({ 1.0f, 1.0f, 1.0f, 1.0f }),
	direction({ 0.0f, 0.0f, 1.0f })
{

}

ZeldaLight::ZeldaLight(const ZeldaLight& zeldaLight) :
	ambient(zeldaLight.ambient),
	diffuse(zeldaLight.diffuse),
	specular(zeldaLight.specular),
	direction(zeldaLight.direction)
{

}

ZeldaLight::~ZeldaLight()
{

}

void ZeldaLight::SetAmbient(float red, float green, float blue, float alpha)
{
	ambient = XMFLOAT4(red, green, blue, alpha);
}

void ZeldaLight::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuse = XMFLOAT4(red, green, blue, alpha);
}

void ZeldaLight::SetSpecular(float red, float green, float blue, float alpha)
{
	specular = XMFLOAT4(red, green, blue, alpha);
}

void ZeldaLight::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT4 ZeldaLight::GetAmbient()
{
	return ambient;
}

DirectX::XMFLOAT4 ZeldaLight::GetDiffuseColor()
{
	return diffuse;
}

DirectX::XMFLOAT4 ZeldaLight::GetSpecular()
{
	return specular;
}

DirectX::XMFLOAT3 ZeldaLight::GetDirection()
{
	return direction;
}
