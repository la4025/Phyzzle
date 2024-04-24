#include "AttachSelectState.h"

namespace Phyzzle
{
	AttachSelectState::~AttachSelectState()
		= default;

	// 상태 들어오기
	void AttachSelectState::StateEnter()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);

			mode = SELECT;
		}
	}

	// 상태 나가기
	void AttachSelectState::StateExit()
	{
		// 플레이어 카메라 컴포넌트를 만든다면
		// 이부분은 수정이 반드시 필요함
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);

			mode = SELECT;
		}

		Reset();
	}

	// Update
	void AttachSelectState::StateStay()
	{
		switch (mode)
		{
		case SELECT:
		{
			SearchDebugDraw(Search());
		}
		break;

		case HOLD:
		case ROTATE:
		{
			ApplyObjectVelocity();
			ResetObjectVelocity();
		}
		break;

		default: ;
		}
	}

#pragma region 키입력
	// 이동
	void AttachSelectState::Stick_L()
	{
		switch (mode)
		{
		case SELECT:
		{
			// 이동 방향으로 캐릭터를 회전 시킴
			PlayerMove(player->data.moveSpeed);
			auto velocity = player->data.playerRigidbody->GetLinearVelocity();
			velocity.y() = 0.f;
			LookToWorldDirection(velocity);
		}
		break;

		case HOLD:
		case ROTATE:
		{

			PlayerMove(player->data.moveSpeed * 0.7f);
			// 오브젝트 방향으로 캐릭터를 회전 시킴
			Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
			direction.y() = 0.f;
			LookToLocalDirection(direction);

			// 플레이어 이동 방향과 같은 방향으로 힘을 줌
			ObjectTranslate(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
		}
		break;

		default:;
		}
	}

	// 카메라 회전
	void AttachSelectState::Stick_R()
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

		switch (mode)
		{
		case SELECT:
		{
			CameraAround();
		}
		break;

		case HOLD:
		case ROTATE:
		{
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
			break;

		default:;
		}
	}

	// 취소
	void AttachSelectState::Click_A()
	{
		StateCancel();
	}

	// 선택
	void AttachSelectState::Click_B()
	{
		switch (mode)
		{
		case SELECT:
			// 레이캐스팅
			if (Select())
			{
				// 정보를 저장함.
				Set();
			}
			break;

		// 오브젝트를 이동시킬수 있는 상태라면
		case HOLD:
		case ROTATE:
			// 붙일 수 있어야함.
			break;

		default:;
		}
	}

	// 취소
	void AttachSelectState::Click_X()
	{
		StateCancel();
	}

	// 취소
	void AttachSelectState::Click_Y()
	{
		StateCancel();
	}

	void AttachSelectState::Pressing_DUp()
	{
		switch (mode)
		{
		case HOLD:
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
		break;

		case ROTATE:
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DDown()
	{
		switch (mode)
		{
		case HOLD:
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
		break;

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DLeft()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_DRight()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Pressing_RB()
	{
		switch (mode)
		{

		case HOLD:

			break;

		default:;
		}
	}

	void AttachSelectState::Click_DUp()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DDown()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DLeft()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	void AttachSelectState::Click_DRight()
	{
		switch (mode)
		{

		case ROTATE:
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
			break;

		default:;
		}
	}

	// 취소
	void AttachSelectState::Click_LB()
	{
		StateCancel();
	}

#pragma endregion 키입력

	void AttachSelectState::PlayerMove(float _speed) const
	{
		player->PlayerMove(_speed);
	}

	void AttachSelectState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachSelectState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttachSelectState::Jump() const
	{
		player->Jump();
	}

	void AttachSelectState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachSelectState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachSelectState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttachSelectState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachSelectState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity);
	}

	void AttachSelectState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
	}

	void AttachSelectState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachSelectState::ObjectRotate()
	{
	}

	void AttachSelectState::Attach() const
	{
	}

	void AttachSelectState::Set()
	{
		using namespace Eigen;

		if (selectBody == nullptr)
			return;

		mode = HOLD;

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

	void AttachSelectState::Reset()
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
		mode = SELECT;
	}

	void AttachSelectState::SearchDebugDraw(bool _value)
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

		if (_value)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"O",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
		else
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"X",
				945, 510,
				200, 100, 50,
				255, 255, 255, 255);
		}
	}
}
