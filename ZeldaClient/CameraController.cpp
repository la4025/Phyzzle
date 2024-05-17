#include "CameraController.h"

namespace Phyzzle
{
	void CameraController::Start()
	{

	}

	Eigen::Vector3f CameraController::GetArmWorldDirection()
	{
		return cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitZ();
	}

	Eigen::Vector3f CameraController::GetCoreWorldDirection()
	{
		return cameraCore->GetWorldRotation() * Eigen::Vector3f::UnitZ();
	}

	void CameraController::ArmReset()
	{

	}

	void CameraController::CoreReset()
	{

	}

	void CameraController::ArmRotateY(float _angle)
	{
		cameraArm->Rotate(Eigen::Vector3f::UnitY(), _angle);
	}

	void CameraController::ArmRotateX(float _angle)
	{
		const Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();

		cameraArm->Rotate(cameraRight, _angle);
	}

	void CameraController::CoreRotateY(float _angle)
	{
		cameraCore->Rotate(Eigen::Vector3f::UnitY(), _angle);
	}

	void CameraController::CoreRotateX(float _angle)
	{
		const Eigen::Vector3f cameraRight = cameraCore->GetWorldRotation() * Eigen::Vector3f::UnitX();

		cameraCore->Rotate(cameraRight, _angle);
	}

	void CameraController::ArmRotate(const Eigen::Vector3f& _axis, float _angle)
	{
		assert(!_axis.isZero());


	}

	void CameraController::CoreRotate(const Eigen::Vector3f& _axis, float _angle)
	{
		assert(!_axis.isZero());

	}

	void CameraController::LookToWorld(const Eigen::Vector3f& _worldDirection)
	{
		assert(!_worldDirection.isZero());


	}

	void CameraController::LookAtWorld(const Eigen::Vector3f& _worldPosition)
	{
		assert(!_worldPosition.isZero());


	}

	void CameraController::LookToLocal(const Eigen::Vector3f& _localDirection)
	{
		assert(!_localDirection.isZero());


	}

	void CameraController::LookAtLocal(const Eigen::Vector3f& _localPosition)
	{
		assert(!_localPosition.isZero());


	}
}
