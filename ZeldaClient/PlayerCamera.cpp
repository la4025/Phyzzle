#include "PlayerCamera.h"

namespace Phyzzle
{
	PlayerCamera::~PlayerCamera()
	{
	}

	void PlayerCamera::Start()
	{

	}

	void PlayerCamera::Update()
	{

	}

	void PlayerCamera::LateUpdate()
	{

	}

	Eigen::Vector3f PlayerCamera::GetArmForward()
	{
		return arm->GetFront();
	}

	Eigen::Vector3f PlayerCamera::GetArmRight()
	{
		return arm->GetRight();
	}

	Eigen::Vector3f PlayerCamera::GetArmUp()
	{
		return arm->GetUp();
	}

	Eigen::Vector3f PlayerCamera::GetCoreForward()
	{
		return core->GetFront();
	}

	Eigen::Vector3f PlayerCamera::GetCoreRight()
	{
		return core->GetRight();
	}

	Eigen::Vector3f PlayerCamera::GetCoreUp()
	{
		return core->GetUp();
	}

	void PlayerCamera::CameraArmRotate()
	{
	}

	void PlayerCamera::CameraArmTranslate()
	{
	}

	void PlayerCamera::CameraCoreRotate()
	{
	}

	void PlayerCamera::CameraCoreTranslate()
	{
	}
}
