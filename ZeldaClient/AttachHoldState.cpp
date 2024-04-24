#include "AttachHoldState.h"

namespace Phyzzle
{
	AttachHoldState::~AttachHoldState()
	= default;

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		if (!TrySelect())
		{
			player->ChangeState(Player::ATTACH_SELECT);
		}
		else
		{
			VariableSet();
		}
	}

	void AttachHoldState::StateExit()
	{
		VariableReset();
	}

	void AttachHoldState::StateStay()
	{
		ApplyObjectVelocity();
		ResetObjectVelocity();
	}
#pragma endregion StateEvent

#pragma region Input
	// 플레이어 이동
	void AttachHoldState::Stick_L()
	{
		//case HOLD:
		//case ROTATE:
		PlayerMove(player->data.moveSpeed * 0.7f);
		// 오브젝트 방향으로 캐릭터를 회전 시킴
		Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		direction.y() = 0.f;
		LookToLocalDirection(direction);

		// 플레이어 이동 방향과 같은 방향으로 힘을 줌
		ObjectTranslate(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
	}

	// 오브젝트 이동
	void AttachHoldState::Stick_R()
	{
		// Stick L은 플레이어의 움직임
		// Stick R와 D Pad는 잡은 오브젝트의 움직임
		//
		// 업데이트 마다 오브젝트와 플레이어의 상대적 거리와 회전을 다시 계산할 것.
		// 업데이트 했을 때 위치가 적합한지 체크하고 아니면 능력을 취소함.
		//
		// Stick L로 플레이어를 움직일 때, 물체에도 같이 힘을 줘야 함.
		//
		// 물체를 선택한 상태라면
		// 좌우 입력은 '플레이어가 보고 있는 방향'의 좌우로 힘을 주고
		// 상하 입력은 월드 Up, Down 방향으로 힘을 주는 것으로 하자.
		// 플레이어가 항상 물체를 바라보고 있는 것으로 하면 얼추 비슷하게 나올 것이다.

		// 키 입력이 없으면 힘 안줌
		if (player->currInput.Rstick.Size)
		{
			const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
			const Eigen::Vector3f objectPos = selectBody->GetPosition();
			Eigen::Vector3f forward = objectPos - playerPos;
			const float distance = forward.y();
			forward.y() = 0.f;
			forward.normalize();
			const Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);
			const Eigen::Vector3f rightVelocity = right * player->currInput.Rstick.X;
			Eigen::Vector3f upVelocity = Eigen::Vector3f::Zero();

			if (player->currInput.Rstick.Y > 0.f && distance < 10.f)
			{
				upVelocity = Eigen::Vector3f::UnitY() * player->currInput.Rstick.Y;
			}
			else if (player->currInput.Rstick.Y < 0.f && distance > -5.f)
			{
				upVelocity = Eigen::Vector3f::UnitY() * player->currInput.Rstick.Y;
			}

			Eigen::Vector3f direction = upVelocity + rightVelocity;
			direction.normalize();
			direction *= player->currInput.Rstick.Size;

			ObjectTranslate(direction, 5.f);
		}
	}

	// 취소
	void AttachHoldState::Click_A()
	{
		StateCancel();
	}

	// 부착
	void AttachHoldState::Click_B()
	{
		// 오브젝트를 이동시킬수 있는 상태라면
		// 붙일 수 있어야함.
	}

	// 취소
	void AttachHoldState::Click_X()
	{
		StateCancel();
	}

	// 취소
	void AttachHoldState::Click_Y()
	{
		StateCancel();
	}

	// 취소
	void AttachHoldState::Click_LB()
	{
		StateCancel();
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DUp()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DDown()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DLeft()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DRight()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DUp()
	{
		// 플레이어 - 오브젝트 방향으로 주고 받음.
		const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const Eigen::Vector3f objectPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objectPos - playerPos;
		forward.y() = 0.f;
		float distance = forward.norm();

		if (distance > 20.f)
			return;

		forward.normalize();

		ObjectTranslate(forward, 10.f);
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DDown()
	{
		Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		Eigen::Vector3f objectPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objectPos - playerPos;
		forward.y() = 0.f;
		float distance = forward.norm();

		if (distance < 3.f)
			return;

		forward.normalize();

		ObjectTranslate(forward, -10.f);
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DLeft()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 -회전 시킴
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DRight()
	{
		//case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
	}

	// Rotate 모드
	void AttachHoldState::Pressing_RB()
	{

	}
#pragma endregion Input

#pragma region Content
	void AttachHoldState::PlayerMove(float _speed) const
	{
		player->PlayerMove(_speed);
	}

	void AttachHoldState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachHoldState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	bool AttachHoldState::TrySelect()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachHoldState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachHoldState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	void AttachHoldState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity);
	}

	void AttachHoldState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
	}

	void AttachHoldState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachHoldState::XRotate() const
	{

	}

	void AttachHoldState::YRotate() const
	{

	}

	void AttachHoldState::Attach() const
	{

	}

	void AttachHoldState::VariableSet()
	{
		using namespace Eigen;

		if (selectBody == nullptr)
			return;

		hasGravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const Eigen::Vector3f playerPosition = player->GetGameObject()->GetTransform()->GetWorldPosition();

		const Eigen::Vector3f lookTo = objectPosition - playerPosition;
		LookToWorldDirection(lookTo);

		distance = lookTo.size();
	}

	void AttachHoldState::VariableReset()
	{
		using namespace Eigen;

		if (selectBody)
		{
			selectBody->UseGravity(hasGravity);
			selectBody->SetMass(mass);
		}

		playerVelocity = Eigen::Vector3f::Zero();
		targetVelocity = Eigen::Vector3f::Zero();
		hasGravity = false;
		mass = -0.1f;
		distance = -0.1f;
		selectBody = nullptr;
	}
#pragma endregion Content

#if _DEBUG
	void AttachHoldState::SearchDebugDraw()
	{
		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"player to object : " +
			std::to_wstring(debugVector0.x()) + L" " +
			std::to_wstring(debugVector0.y()) + L" " +
			std::to_wstring(debugVector0.z()) + L" ",
			1200, 100,
			200, 600, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"world direction : " +
			std::to_wstring(debugVector1.x()) + L" " +
			std::to_wstring(debugVector1.y()) + L" " +
			std::to_wstring(debugVector1.z()) + L" ",
			1200, 200,
			200, 600, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"local direction : " +
			std::to_wstring(debugVector2.x()) + L" " +
			std::to_wstring(debugVector2.y()) + L" " +
			std::to_wstring(debugVector2.z()) + L" ",
			1200, 300,
			200, 600, 15,
			255, 255, 255, 255);
	}
#endif _DEBUG
}
