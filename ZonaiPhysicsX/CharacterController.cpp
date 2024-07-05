#include "PxPhysicsAPI.h"

#include "ZnUtil.h"
#include "CharacterController.h"


namespace ZonaiPhysics
{
	CharacterController::CharacterController(physx::PxController* _controller) : controller(_controller)
	{

	}

	void CharacterController::SetPosition(const Eigen::Vector3f& _position)
	{
		controller->setPosition({ _position.x(), _position.y(), _position.z() });
	}

	void CharacterController::SetFootPosition(const Eigen::Vector3f& _position)
	{
		controller->setFootPosition({ _position.x(), _position.y(), _position.z() });
	}

	void CharacterController::SetStepOffset(const float _offset)
	{
		controller->setStepOffset(_offset);
	}

	void CharacterController::SetContactOffset(const float _offset)
	{
		controller->setContactOffset(_offset);
	}

	void CharacterController::SetUpDirection(const Eigen::Vector3f& _direction)
	{
		controller->setUpDirection({ _direction.x(), _direction.y(), _direction.z() });
	}

	void CharacterController::SetSlope(const float _slope)
	{
		controller->setSlopeLimit(_slope);
	}

	Eigen::Vector3f CharacterController::GetPosition()
	{
		return PhysxToEigen(physx::toVec3(controller->getPosition()));
	}

	Eigen::Vector3f CharacterController::GetFootPosition()
	{
		return PhysxToEigen(physx::toVec3(controller->getFootPosition()));
	}

	float CharacterController::GetStepOffset()
	{
		return controller->getStepOffset();
	}

	float CharacterController::GetContactOffset()
	{
		return controller->getContactOffset();
	}

	Eigen::Vector3f CharacterController::GetUpDirection()
	{
		return PhysxToEigen(controller->getUpDirection());
	}

	float CharacterController::GetSlope()
	{
		return controller->getSlopeLimit();
	}

	bool CharacterController::Move(Eigen::Vector3f _vec, float _dis, float _dt)
	{
		physx::PxControllerCollisionFlags flag = controller->move(
			EigenToPhysx(_vec),
			_dis,
			_dt,
			physx::PxControllerFilters()
		);

		return true;
	}
}