#include "ZeldaLight.h"

using namespace DirectX;

ZeldaLight::ZeldaLight(LightType type) :
	ambient({ 0.2f, 0.2f, 0.2f, 1.0f }),
	diffuse({ 1.0f, 1.0f, 1.0f, 1.0f }),
	specular({ 1.0f, 1.0f, 1.0f, 1.0f }),
	position({ 0.0f, 0.0f, 0.0f, 0.0f }),
	direction({ 0.0f, 0.0f, 1.0f, 0.0f }),
	type(type),
	range(10.0f),
	angle(3.141592f * 0.1666f)
{

}

ZeldaLight::~ZeldaLight()
{

}

void ZeldaLight::SetAmbient(float red, float green, float blue)
{
	ambient = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetDiffuseColor(float red, float green, float blue)
{
	diffuse = XMFLOAT4(red, green, blue, 1.0f);
}

void ZeldaLight::SetSpecular(float red, float green, float blue)
{
	specular = XMFLOAT4(red, green, blue, 1.0f);
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

void ZeldaLight::SetDirection(float x, float y, float z)
{
	assert(type == LightType::Directional);
	direction = XMFLOAT4(x, y, z, 0.0f);
}

DirectX::XMFLOAT3 ZeldaLight::GetDirection()
{
	assert(type == LightType::Directional);
	return XMFLOAT3(direction.x, direction.y, direction.z);
}

void ZeldaLight::SetRange(float range)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->range = range;
}

void ZeldaLight::SetPosition(float x, float y, float z)
{
	assert(type == LightType::Point || type == LightType::Spot);
	this->position = XMFLOAT4(x, y, z, 0.0f);
}

float ZeldaLight::GetRange()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return range;
}

DirectX::XMFLOAT3 ZeldaLight::GetPosition()
{
	assert(type == LightType::Point || type == LightType::Spot);
	return XMFLOAT3(position.x, position.y, position.z);
}

void ZeldaLight::SetAngle(float angle)
{
	assert(type == LightType::Spot);
	this->angle = angle;
}

float ZeldaLight::GetAngle()
{
	assert(type == LightType::Spot);
	return angle;
}

LightType ZeldaLight::GetLightType()
{
	return type;
}

LightInfo ZeldaLight::GetLightInfo()
{
	LightInfo lightInfo;
	lightInfo.color.ambient = ambient;
	lightInfo.color.diffuse = diffuse;
	lightInfo.color.specular = specular;
	lightInfo.position = position;
	lightInfo.direction = direction;
	lightInfo.type = type;
	lightInfo.range = range;
	lightInfo.angle = angle;

	return lightInfo;
}
