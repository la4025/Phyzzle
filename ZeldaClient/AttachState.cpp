#include "AttachState.h"

namespace Phyzzle
{
	AttachState::~AttachState()
		= default;

	// 상태 들어오기
	void AttachState::StateEnter()
	{
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}
	}

	// 상태 나가기
	void AttachState::StateExit()
	{
		// 플레이어 카메라 컴포넌트를 만든다면
		// 이부분은 수정이 반드시 필요함
		{
			auto p = player->data.cameraCore->GetLocalPosition();
			p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

			player->data.cameraCore->SetLocalPosition(p);
		}

		Reset();
	}

	// Update
	void AttachState::StateStay()
	{
		if (selected)
		{
			// 오브젝트의 위치를 업데이트 함.

		}
		else
		{
			SearchDebugDraw(Search());
		}
	}

#pragma region 키입력
	// 이동
	void AttachState::Stick_L()
	{
		PlayerMove();

		if (selected)
		{
			// 카메라 방향으로 캐릭터를 회전 시킴
			// auto direction = player->data.cameraArm->GetLocalRotation() * Eigen::Vector3f::UnitZ();
			// 오브젝트 방향으로 캐릭터를 회전 시킴
			Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
			direction.y() = 0.f;

			LookToLocalDirection(direction);
		}
		else
		{
			// 이동 방향으로 캐릭터를 회전 시킴
			auto velocity = player->data.playerRigidbody->GetLinearVelocity();
			velocity.y() = 0.f;

			LookToWorldDirection(velocity);
		}
	}

	// 카메라 회전
	void AttachState::Stick_R()
	{
		// Stick L은 플레이어의 움직임
		// Stick R와 D Pad는 잡은 오브젝트의 움직임

		// 업데이트 마다 오브젝트와 플레이어의 상대적 거리와 회전을 다시 계산할 것.
		// 업데이트 했을 때 위치가 적합한지 체크하고 아니면 능력을 취소함.

		// Stick L로 플레이어를 움직일 때, 물체에도 같이 힘을 줘야 함.

		// 물체를 선택한 상태라면
		// 좌우 입력은 '플레이어가 보고 있는 방향'의 좌우로 힘을 주고
		// 상하 입력은 월드 Up, Down 방향으로 힘을 주는 것으로 하자.
		// 플레이어가 항상 물체를 바라보고 있는 것으로 하면 얼추 비슷하게 나올 것이다.

		if (!selected)
		{
			CameraAround();
		}
		else
		{
			if (player->currInput.Rstick.Size)
			{
				Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
				Eigen::Vector3f objectPos = selectBody->GetPosition();
				Eigen::Vector3f forward = objectPos - playerPos;
				forward.y() = 0.f;
				forward.normalize();
				Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);
				Eigen::Vector3f direction = Eigen::Vector3f::UnitY() * player->currInput.Rstick.Y + right * player->currInput.Rstick.X;
				direction.normalize();
				direction *= player->currInput.Rstick.Size;

				ObjectTranslate(direction, 5.f);
			}
		}
	}

	// 취소
	void AttachState::Click_A()
	{
		StateCancel();
	}

	// 선택
	void AttachState::Click_B()
	{
		// 울트라 핸드 상태가 아니라면..
		if (!selected)
		{
			if (Select())
			{
				Set();
			}
		}
		// 오브젝트를 이동시킬수 있는 상태라면
		else
		{
			// 붙일 수 있어야함.
		}
	}

	// 취소
	void AttachState::Click_X()
	{
		StateCancel();
	}

	// 취소
	void AttachState::Click_Y()
	{
		StateCancel();
	}

	void AttachState::Click_DUp()
	{
		if (!rotateMode)
		{
			Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
			Eigen::Vector3f objectPos = selectBody->GetPosition();
			Eigen::Vector3f forward = objectPos - playerPos;
			forward.y() = 0.f;
			forward.normalize();

			ObjectTranslate(forward, 5.f);
		}
		else
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 +회전 시킴
		}
	}

	void AttachState::Click_DDown()
	{
		if (!rotateMode)
		{
			Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
			Eigen::Vector3f objectPos = selectBody->GetPosition();
			Eigen::Vector3f forward = objectPos - playerPos;
			forward.y() = 0.f;
			forward.normalize();

			ObjectTranslate(forward, -5.f);
		}
		else
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 -회전 시킴
		}
	}

	void AttachState::Click_DLeft()
	{
		if (rotateMode)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
		}
	}

	void AttachState::Click_DRight()
	{
		if (rotateMode)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴
		}
	}

	// 취소
	void AttachState::Click_LB()
	{
		StateCancel();
	}

	// 취소
	void AttachState::Click_RB()
	{
		StateCancel();
	}

#pragma endregion 키입력

	void AttachState::PlayerMove() const
	{
		player->PlayerMove(player->data.moveSpeed);
	}

	void AttachState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttachState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttachState::Jump() const
	{
		player->Jump();
	}

	void AttachState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
		debugVector1 = _to.normalized();

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
		debugVector2 = _to.normalized();

		player->LookInLocalDirection(_to.normalized());
	}

	bool AttachState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttachState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttachState::ApplyPlayerVelocity() const
	{
		player->data.playerRigidbody->SetLinearVelocity(playerVelocity);
	}

	void AttachState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity);
	}

	void AttachState::ObjectTranslate(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachState::Set()
	{
		using namespace Eigen;

		if (selectBody == nullptr)
			return;

		selected = true;

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

	void AttachState::Reset()
	{
		using namespace Eigen;

		if (selectBody && selected)
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
		selected = false;
	}

	void AttachState::SearchDebugDraw(bool _value)
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
