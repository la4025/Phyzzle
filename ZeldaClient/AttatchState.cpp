#include "AttatchState.h"

namespace Phyzzle
{
	AttatchState::~AttatchState()
		= default;

	// 상태 들어오기
	void AttatchState::StateEnter()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p += Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);
	}

	// 상태 나가기
	void AttatchState::StateExit()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);

		Reset();
	}

	// Update
	void AttatchState::StateStay()
	{
		if (selected)
		{
			// 오브젝트의 위치를 업데이트 함.
			ObjectToTargetPosition();
		}
		else
		{
			SearchDebugDraw(Search());
		}
	}

#pragma region 키입력
	// 이동
	void AttatchState::Stick_L()
	{
		PlayerMove();

		if (selected)
		{
			// 카메라 방향으로 캐릭터를 회전 시킴
			auto direction = player->data.cameraArm->GetLocalRotation() * Eigen::Vector3f::UnitZ();
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
	void AttatchState::Stick_R()
	{
		// 물체를 선택한 상태라면
		// 좌우 입력은 '플레이어가 보고 있는 방향'의 좌우로 힘을 주고
		// 상하 입력은 월드 Up, Down 방향으로 힘을 주는 것으로 하자.
		// 플레이어가 항상 물체를 바라보고 있는 것으로 하면 얼추 비슷하게 나올 것이다.
		CameraAround();
	}

	// 취소
	void AttatchState::Click_A()
	{
		StateCancel();
	}

	// 선택
	void AttatchState::Click_B()
	{
		if (Select())
		{
			Set();
		}
	}

	// 취소
	void AttatchState::Click_X()
	{
		StateCancel();
	}

	// 취소
	void AttatchState::Click_Y()
	{
		StateCancel();
	}

	// 취소
	void AttatchState::Click_LB()
	{
		StateCancel();
	}

	// 취소
	void AttatchState::Click_RB()
	{
		StateCancel();
	}
#pragma endregion 키입력

	void AttatchState::PlayerMove() const
	{
		player->Move(player->data.moveSpeed);
	}

	void AttatchState::CameraAround() const
	{
		player->CameraAround();
	}

	void AttatchState::StateCancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttatchState::Jump() const
	{
		player->Jump();
	}

	void AttatchState::LookToWorldDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInWorldDirection(_to.normalized());
	}

	void AttatchState::LookToLocalDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInLocalDirection(_to.normalized());
	}

	bool AttatchState::Search() const
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, nullptr, nullptr, nullptr);

		if (!hit)
			return false;

		return true;
	}

	bool AttatchState::Select()
	{
		const float distance = 40.f;
		const bool hit = player->CameraForwardRaycast(distance, &selectBody, nullptr, nullptr);

		if (!hit)
			return false;

		if (!selectBody)
			return false;

		return true;
	}

	void AttatchState::ObjectToTargetPosition() const
	{
		using namespace Eigen;

		const Vector3f currPosition = selectBody->GetPosition();
		const Quaternionf currRotation = selectBody->GetRotation();

		const Vector3f targetP = MulMatrixVector(player->data.modelCore->GetWorldMatrix(), targetLocalPosition);
		const Quaternionf targetR = player->data.modelCore->GetWorldRotation() * targetLocalRotation;

		const Vector3f diffP = targetP - currPosition;
		const Quaternionf diffR = currRotation.inverse() * targetR;

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"현재 회전 값 : " +
			std::to_wstring(currRotation.vec().x()) + L" " +
			std::to_wstring(currRotation.vec().y()) + L" " +
			std::to_wstring(currRotation.vec().z()) + L" ",
			1200, 100,
			200, 300, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"목표 회전 값 : " +
			std::to_wstring(targetR.vec().x()) + L" " + 
			std::to_wstring(targetR.vec().y()) + L" " + 
			std::to_wstring(targetR.vec().z()) + L" ",
			1200, 150,
			200, 300, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"회전 차이 : " + 
			std::to_wstring(diffR.vec().x()) + L" " +
			std::to_wstring(diffR.vec().y()) + L" " +
			std::to_wstring(diffR.vec().z()) + L" ",
			1200, 200,
			200, 300, 15,
			255, 255, 255, 255);

		selectBody->SetLinearVelocity(diffP / lerpTime);
		selectBody->SetRotation(targetR);
	}

	void AttatchState::Set()
	{
		if (selectBody == nullptr)
			return;

		using namespace Eigen;
		selected = true;

		hasGravity = selectBody->HasGravity();
		mass = selectBody->GetMass();

		selectBody->UseGravity(false);
		selectBody->SetMass(0.001f);

		Eigen::Vector3f lookTo = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		lookTo.normalize();
		// 모델을 물체를 바라보도록 함.
		LookToWorldDirection(lookTo);

		targetLocalPosition = MulMatrixVector(player->data.modelCore->GetWorldMatrix().inverse(), selectBody->GetPosition());
		targetLocalRotation = player->data.modelCore->GetWorldRotation().inverse() * selectBody->GetRotation();
	}

	void AttatchState::Reset()
	{
		using namespace Eigen;

		if (selectBody && selected)
		{
			selectBody->UseGravity(hasGravity);
			selectBody->SetMass(mass);
		}

		targetLocalPosition = Vector3f::Zero();
		targetLocalRotation = Quaternionf::Identity();
		hasGravity = false;
		mass = -0.1f;
		selectBody = nullptr;
		selected = false;
	}

	void AttatchState::SearchDebugDraw(bool _value)
	{
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
