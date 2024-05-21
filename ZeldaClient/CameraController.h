#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Transform;
}

namespace Phyzzle
{
	class CameraController : public PurahEngine::Component
	{
	public:
		~CameraController() override;

	public:
		void Start();

	public:
		Eigen::Vector3f GetArmWorldDirection();
		Eigen::Vector3f GetCoreWorldDirection();

		void ArmReset();
		void CoreReset();

		void ArmRotateY(float _angle);
		void ArmRotateX(float _angle);

		void CoreRotateY(float _angle);
		void CoreRotateX(float _angle);

		void ArmRotate(const Eigen::Vector3f& _axis, float _angle);
		void CoreRotate(const Eigen::Vector3f& _axis, float _angle);

		void LookToWorld(const Eigen::Vector3f& _worldDirection);
		void LookAtWorld(const Eigen::Vector3f& _worldPosition);

		void LookToLocal(const Eigen::Vector3f& _localDirection);
		void LookAtLocal(const Eigen::Vector3f& _localPosition);

	private:
		PurahEngine::Transform* cameraArm;
		PurahEngine::Transform* cameraCore;

	private:
		Eigen::Vector3f armOriginP;
		Eigen::Quaternionf armOriginR;
	};
}

