#include "DirectionalLight.h"

PurahEngine::DirectionalLight::DirectionalLight()
= default;

PurahEngine::DirectionalLight::~DirectionalLight()
= default;

void PurahEngine::DirectionalLight::Awake()
{
	
}

void PurahEngine::DirectionalLight::Update()
{

}

void PurahEngine::DirectionalLight::SetLightAmbient(const Eigen::Vector3f& _ambient)
{
	if (awake)
	{

	}
	else
	{
		ambient = _ambient;
	}
}

void PurahEngine::DirectionalLight::SetLightDiffuse(const Eigen::Vector3f& _diffuse)
{
	if (awake)
	{
		
	}
	else
	{
		diffuse = _diffuse;
	}
}

void PurahEngine::DirectionalLight::SetLightSpecular(const Eigen::Vector3f& _specular)
{
	if (awake)
	{
		
	}
	else
	{
		specular = _specular;
	}
}

void PurahEngine::DirectionalLight::SetLightDirection(const Eigen::Vector3f& _direction)
{
	if (awake)
	{

	}
	else
	{
		direction = _direction;
	}
}
