#include "PlayerController.h"


namespace Phyzzle
{
	void PlayerController::Start()
	{
		gameObject->AddComponent<PurahEngine::RigidBody>();
		auto capsule = gameObject->AddComponent<PurahEngine::CapsuleCollider>();
		capsule->radius;
		capsule->height;
	}

	void PlayerController::Update()
	{
		groundCheck.direction = -1.f * Eigen::Vector3f::UnitY();
		groundCheck.distance;
	}

	void PlayerController::Move(const Eigen::Vector3f& _dir, float _speed)
	{
		const Eigen::Vector3f movement = _dir * _speed;

		// sweep해서 갈 수 있는 곳인지 아닌지 체크
	}

	void PlayerController::LookTo(const Eigen::Vector3f& _worldDirection)
	{
		assert(!_worldDirection.isZero());

		// 플레이어의 월드 회전을 구함
		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();

		// Model의 로컬 방향을 구함.
		const Eigen::Vector3f localDirection = parentWorld.conjugate() * _worldDirection;

		// 로컬 Z와 Local Direction 사이의 쿼터니언을 구함.
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localDirection);

		// Model을 회전시킴
		model->SetLocalRotation(targetRotation);
	}

	void PlayerController::LookAt(const Eigen::Vector3f& _worldPosition)
	{
		const Eigen::Vector3f position = gameObject->GetTransform()->GetWorldPosition();

		Eigen::Vector3f to = _worldPosition - position;
		to.normalize();

		LookTo(to);
	}

	void PlayerController::Jump()
	{
		if (!CanJump())
			return;

		Eigen::Vector3f power = Eigen::Vector3f::UnitY() * jumpPower;
		body->AddForce(power, ZonaiPhysics::ForceType::Accelration);
		jumping = true;
	}

	void PlayerController::SetSlope(float _angle)
	{
		slope = _angle;
	}

	bool PlayerController::CanJump() const
	{
		return jumping == false;
	}

	bool PlayerController::IsGround()
	{
		// PurahEngine::Physics::Capsulecast()

		return true;
	}

	bool PlayerController::IsWall()
	{


		return true;
	}

	bool PlayerController::IsObstacle()
	{


		return true;
	}

	void PlayerController::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{
		for (size_t i = 0; i < zn_collision.contactCount; i++)
		{

		}
	}

	void PlayerController::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{

	}

	void PlayerController::OnCollisionExit(const ZonaiPhysics::ZnCollision& zn_collision,
		const PurahEngine::Collider* collider)
	{

	}
}
