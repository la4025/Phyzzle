#pragma once
#include "Component.h"
#include <Eigen/Dense>

namespace PurahEngine
{
	class DirectionalLight : public Component
	{
	public:
		DirectionalLight();
		~DirectionalLight() override;

	public:
		void Awake() override;
		void Update() override;

	public:
		void SetLightAmbient(const Eigen::Vector3f&);
		void SetLightDiffuse(const Eigen::Vector3f&);
		void SetLightSpecular(const Eigen::Vector3f&);
		void SetLightDirection(const Eigen::Vector3f&);

	public:
		bool awake = true;
		Eigen::Vector3f ambient;
		Eigen::Vector3f diffuse;
		Eigen::Vector3f specular;
		Eigen::Vector3f direction;
	};
}

