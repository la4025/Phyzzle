#include "AttachHoldState.h"

#include "PzObject.h"
#include "AttachSystem.h"

namespace Phyzzle
{
	AttachHoldState::AttachHoldState(Player* _player)
		: IState(_player), selectBody(), attachble()
	{
		using namespace Eigen;

		std::vector<Quaternionf> axis;
		InitializeAxis(axis, 90);

		// none
		std::vector<Quaternionf> none;
		{
			for (size_t i = 0; i < axis.size(); i++)
			{
				Quaternionf newAxis = axis[i];
				for (size_t z = 0; z < 360; z += 90)
				{
					float angleZ = (static_cast<float>(z) / 180.f) * std::numbers::pi_v<float>;
					Quaternionf axisZ = Quaternionf(AngleAxisf(angleZ, Vector3f::UnitZ()));

					Quaternionf temp = axisZ * newAxis;
					none.emplace_back(temp);
					axisies.emplace_back(temp, None);
				}
			}
		}

		// X
		std::vector<Quaternionf> rotateX;
		InitializeRotations(none, rotateX, 45, Vector3f::UnitX(), RotateX);

		// Y
		std::vector<Quaternionf> rotateY;
		InitializeRotations(none, rotateY, 45, Vector3f::UnitY(), RotateY);
		InitializeRotations(rotateX, 45, Vector3f::UnitY(), RotateXY);
		InitializeRotations(rotateX, -45, Vector3f::UnitY(), RotateX_Y);
		InitializeRotations(rotateY, 45, Vector3f::UnitX(), RotateYX);
		InitializeRotations(rotateY, -45, Vector3f::UnitX(), RotateY_X);
		InitializeRotations(none, 45, Vector3f::UnitZ(), RotateZ);
	}

	AttachHoldState::~AttachHoldState()
	{
		AttachSystem::Instance()->Clear();
	}

	void AttachHoldState::InitializeAxis(std::vector<Eigen::Quaternionf>& axis, float increment) 
	{
		using namespace Eigen;
		Quaternionf classic = Quaternionf::Identity();

		for (size_t x = 0; x < 360; x += increment)
		{
			float angleY = (static_cast<float>(x) / 180.f) * std::numbers::pi_v<float>;
			Quaternionf axisY = Quaternionf(AngleAxisf(angleY, Vector3f::UnitY()));
			Quaternionf temp = axisY * classic;
			axis.emplace_back(temp);
		}

		InitializeAxisHelper(axis, -90.f, Vector3f::UnitX());
		InitializeAxisHelper(axis, 90.f, Vector3f::UnitX());
	}

	void AttachHoldState::InitializeAxisHelper(std::vector<Eigen::Quaternionf>& axis, float angle, const Eigen::Vector3f& vector) 
	{
		using namespace Eigen;
		Quaternionf classic = Quaternionf::Identity();

		float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
		Quaternionf axisAngle = Quaternionf(AngleAxisf(angleRad, vector));
		Quaternionf temp = axisAngle * classic;
		axis.emplace_back(temp);
	}

	void AttachHoldState::InitializeRotations(
		const std::vector<Eigen::Quaternionf>& input, 
		std::vector<Eigen::Quaternionf>& output, float angle, 
		const Eigen::Vector3f& axis, RotateInfo type)
	{
		for (const auto& quat : input)
		{
			float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
			Eigen::Quaternionf axisQuat = Eigen::Quaternionf(Eigen::AngleAxisf(angleRad, axis));

			Eigen::Quaternionf temp = axisQuat * quat;
			output.emplace_back(temp);
			axisies.emplace_back(temp, type);
		}
	}

	void AttachHoldState::InitializeRotations(
		const std::vector<Eigen::Quaternionf>& input, 
		float angle, const Eigen::Vector3f& axis, RotateInfo type)
	{
		for (const auto& quat : input)
		{
			float angleRad = (angle / 180.f) * std::numbers::pi_v<float>;
			Eigen::Quaternionf axisQuat = Eigen::Quaternionf(Eigen::AngleAxisf(angleRad, axis));

			Eigen::Quaternionf temp = axisQuat * quat;
			axisies.emplace_back(temp, type);
		}
	}

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		{

		}

		TrySelect();
	}

	void AttachHoldState::StateExit()
	{
		{
			using namespace Eigen;
			player->data.cameraCore->SetLocalPosition(player->data.coreDefaultPosition);
			player->data.cameraCore->SetLocalRotation(player->data.coreDefaultRotation);
			player->data.cameraArm->SetLocalPosition(player->data.armDefaultPosition);

			auto rot = player->data.modelCore->GetLocalRotation();
			player->data.cameraArm->SetLocalRotation(rot);

			player->data.xAngle = 0.f;
		}

		EnableOutline(false);
		VariableReset();
	}

	void AttachHoldState::StateStay()
	{
		CalculateSpringForces();
		ApplyObjectVelocity();
		ResetObjectVelocity();

		UpdateCamera();

		auto euler = targetRotation.toRotationMatrix().eulerAngles(0, 1, 2);
		auto degree = Eigen::Vector3f(
			euler.x() * 180.f / std::numbers::pi_v<float>,
			euler.y() * 180.f / std::numbers::pi_v<float>,
			euler.z() * 180.f / std::numbers::pi_v<float>
			);
		degree.x() = std::floor(degree.x() * 10.f) / 10.f;
		degree.y() = std::floor(degree.y() * 10.f) / 10.f;
		degree.z() = std::floor(degree.z() * 10.f) / 10.f;
		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"오브젝트 각도 : \n" +
			std::to_wstring(degree.x()) + L" \n" +
			std::to_wstring(degree.y()) + L" \n" +
			std::to_wstring(degree.z()) + L" \n",
			1400, 100,
			200, 600, 15,
			255, 255, 255, 255);

#if _DEBUG
		SearchDebugDraw();
#endif _DEBUG
	}
#pragma endregion StateEvent

#pragma region Input
	// 플레이어 이동
	void AttachHoldState::Stick_L()
	{
		PlayerMove(player->data.moveSpeed * 0.7f);

		// 오브젝트 방향으로 캐릭터를 회전 시킴
		Eigen::Vector3f direction = selectBody->GetPosition() - player->GetGameObject()->GetTransform()->GetWorldPosition();
		direction.y() = 0.f;
		LookToLocalDirection(direction);

		// 플레이어 이동 방향과 같은 방향으로 힘을 줌
		TranslateObject(player->data.playerRigidbody->GetLinearVelocity(), 1.f);
	}

	// 오브젝트 이동
	void AttachHoldState::Stick_R()
	{
		// 키 입력이 없으면 힘 안줌
		if (player->currInput.Rstick.Size)
		{
			TranslateObjectAlongX(5.f * player->currInput.Rstick.X * player->currInput.Rstick.Size);
			TranslateObjectAlongY(5.f * player->currInput.Rstick.Y * player->currInput.Rstick.Size);
		}
	}

	void AttachHoldState::Trigger_L()
	{
		adjustmentMode = false;

		if (player->currInput.LTrigger > 0)
			adjustmentMode = true;
	}

	// 취소
	void AttachHoldState::Click_A()
	{
		Put();
		StateCancel();
	}

	// 부착
	void AttachHoldState::Click_B()
	{
		// 오브젝트를 이동시킬수 있는 상태라면
		// 붙일 수 있어야함.
		if (TryAttach())
		{
			Put();
			StateCancel();
		}
	}

	// 취소
	void AttachHoldState::Click_X()
	{
		Put();
		StateCancel();
	}

	// 취소
	void AttachHoldState::Click_Y()
	{
		TryDettach();
	}

	// 취소
	void AttachHoldState::Click_LB()
	{
		Put();
		StateCancel();
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DUp()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			TranslateSpringAlongZ(2.f);
		}
		else if (justRotate)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 +회전 시킴

			switch(info.info)
			{
			case None:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateX;
				break;

			case RotateX:
				RotateSpringAlongX(rotateAngle);
				info.info = None;
				break;

			case RotateY:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateYX;
				break;

			case RotateX_Y:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 왼쪽에 있을 때
				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(-rotateAngle);	// 우
				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(rotateAngle);		// 좌
				info.info = RotateXY;
				break;

			case RotateXY:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 오른쪽에 있을 때
				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(rotateAngle);		// 좌
				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(-rotateAngle);	// 우
				info.info = RotateX_Y;
				break;

			case RotateY_X:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateY;
				break;

			case RotateYX:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateZ;
				break;

			case RotateZ:
				RotateSpringAlongX(rotateAngle);
				info.info = RotateY_X;
				break;

			default: ;
				assert(0);
			}
		}
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DDown()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			TranslateSpringAlongZ(-2.f);
		}
		else if (justRotate)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 -회전 시킴

			switch (info.info)
			{
			case None:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateX;
				break;

			case RotateX:
				RotateSpringAlongX(-rotateAngle);
				info.info = None;
				break;

			case RotateY:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateY_X;
				break;

			case RotateX_Y:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 왼쪽에 있을 때
				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(-rotateAngle);	// 우
				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(rotateAngle);		// 좌
				info.info = RotateXY;
				break;

			case RotateXY:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 오른쪽에 있을 때
				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(rotateAngle);		// 좌
				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(-rotateAngle);	// 우
				info.info = RotateX_Y;
				break;

			case RotateY_X:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateZ;
				break;

			case RotateYX:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateY;
				break;

			case RotateZ:
				RotateSpringAlongX(-rotateAngle);
				info.info = RotateYX;
				break;

			default:;
				assert(0);
			}
		}
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DLeft()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		if (justRotate)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴

			switch (info.info)
			{
			case None:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateY;
				break;

			case RotateX:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateXY;
				break;

			case RotateY:
				RotateSpringAlongY(rotateAngle);
				info.info = None;
				break;

			case RotateX_Y:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateX;
				break;

			case RotateXY:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateZ;
				break;

			case RotateY_X:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 위에 있을 때
				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(-rotateAngle);	// 하
				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(rotateAngle);		// 상

				info.info = RotateYX;
				break;

			case RotateYX:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 아래에 있을 때
				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(rotateAngle);		// 상
				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(rotateAngle);		// 좌

				RotateSpringAlongX(-rotateAngle);	// 하

				info.info = RotateY_X;
				break;

			case RotateZ:
				RotateSpringAlongY(rotateAngle);
				info.info = RotateX_Y;
				break;

			default:;
				assert(0);
			}
		}
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DRight()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		if (justRotate)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴

			switch (info.info)
			{
			case None:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateY;
				break;

			case RotateX:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateX_Y;
				break;

			case RotateY:
				RotateSpringAlongY(-rotateAngle);
				info.info = None;
				break;

			case RotateX_Y:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateZ;
				break;

			case RotateXY:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateX;
				break;

			case RotateY_X:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 위에 있을 때
				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(-rotateAngle);	// 하
				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(rotateAngle);		// 상

				info.info = RotateYX;
				break;

			case RotateYX:
				// 문제의 그 각도인데
				// 메인 면이 되는 게 아래에 있을 때
				RotateSpringAlongX(-rotateAngle);	// 하

				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(rotateAngle);		// 상
				RotateSpringAlongX(rotateAngle);		// 상

				RotateSpringAlongY(-rotateAngle);	// 우

				RotateSpringAlongX(-rotateAngle);	// 하
				info.info = RotateY_X;
				break;

			case RotateZ:
				RotateSpringAlongY(-rotateAngle);
				info.info = RotateXY;
				break;

			default:;
				assert(0);
			}
		}
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DUp()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;
		const bool translateAdjustment = adjustmentMode && !roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (translateAdjustment)
		{
			// 플레이어 - 오브젝트 방향으로 주고 받음.
			const float velocity = pushingVelocity * player->currInput.LTrigger * dt;

			TranslateSpringAlongZ(velocity);
		}
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			RotateSpringAlongX(velocity);
		}
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Pressing_DDown()
	{
		const bool translateAdjustment = adjustmentMode && !roateMode;
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// 이동
		if (translateAdjustment)
		{
			const float velocity = pushingVelocity * player->currInput.LTrigger * dt;

			TranslateSpringAlongZ(-velocity);
		}
		// 회전
		else if (rotateAdjustment)
		{
			const float velocity = rotateAngle * player->currInput.LTrigger * dt;
			
			RotateSpringAlongX(-velocity);
		}
	}

	// 오브젝트 회전
	void AttachHoldState::Pressing_DLeft()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (rotateAdjustment)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 X 축 기준으로 +회전 시킴

			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			RotateSpringAlongY(velocity);
		}
	}

	// 오브젝트 회전
	void AttachHoldState::Pressing_DRight()
	{
		const bool rotateAdjustment = adjustmentMode && roateMode;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (rotateAdjustment)
		{
			// 오브젝트에 각속도를 줘서 회전시킴
			// 플레이어 기준으로 Y 축 기준으로 -회전 시킴

			const float velocity = rotateAngle * player->currInput.LTrigger * dt;

			RotateSpringAlongY(-velocity);
		}
	}

	// Rotate 모드
	void AttachHoldState::Pressing_RB()
	{
		roateMode = true;
	}

	void AttachHoldState::Up_RB()
	{
		roateMode = false;
	}
#pragma endregion Input

#pragma region Content
	void AttachHoldState::PlayerMove(float _speed) const
	{
		player->TryPlayerMove(_speed);
	}

	/*
	void AttachHoldState::CameraUpdate() const
	{
		using namespace Eigen;

		float offset = 5.f;
		Vector3f bodyPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		Vector3f cameraArmPos = player->data.cameraArm->GetWorldPosition();

		// 카메라 암이 오브젝트를 향하도록 함
		Vector3f armDirection = bodyPos - playerPos;
		armDirection.y() = 0.f;
		armDirection.normalize();
		player->CameraLookTo(armDirection);

		/// 임시
		// 카메라가 오브젝트 높이를 따라가도록 함
		float gap = bodyPos.y() - playerPos.y();
		if (gap < 9.f)
		{
			gap = 9.f;
		}
		Vector3f cameraCorePos = cameraArmPos + armDirection * -gap;
		cameraCorePos.y() = bodyPos.y() + 5.f;
		player->data.cameraCore->SetWorldPosition(cameraCorePos);


		// 플레이어와 오브젝트 사이를 바라보게 함
		Vector3f playerToBody = bodyPos - playerPos;
		Vector3f direction = playerToBody.normalized();

		bodyPos = bodyPos + direction * 5.f;
		playerPos = playerPos - direction * 5.f;

		Vector3f to = (bodyPos - playerPos) / 2.f;
		Vector3f focus = playerPos + to;
		player->CameraLookAt(focus);
	}
	*/

	void AttachHoldState::UpdateCamera()
	{
		UpdateCameraPosition();
		UpdateCameraFocus();
	}

	void AttachHoldState::UpdateCameraPosition() const
	{
		using namespace Eigen;

		float offset = 5.f;
		Vector3f bodyPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		Vector3f cameraArmPos = player->data.cameraArm->GetWorldPosition();

		Vector3f armDirection = bodyPos - playerPos;
		armDirection.y() = 0.f;
		armDirection.normalize();
		player->CameraLookTo(armDirection);

		float gap = bodyPos.y() - playerPos.y();
		if (gap < 9.f)
		{
			gap = 9.f;
		}
		Vector3f cameraCorePos = cameraArmPos + armDirection * -gap;
		cameraCorePos.y() = bodyPos.y() + 5.f;
		player->data.cameraCore->SetWorldPosition(cameraCorePos);

		// 오브젝트의 y 위치에 따라서 카메라 동작이 조금 다르게 될 것.
		// y가 플레이어보다 낮으면 카메라가 위에서 플레이어를 향해서 내려다보는 형태
		// y가 플레이어보다 높으면 카메라는 오브젝트와 동일한 높이에서 시작해서
		// 카메라에 오브젝트와 플레이어가 모두 들어갈 수 있도록 위치를 조정한다.

	}

	void AttachHoldState::UpdateCameraFocus() const
	{
		using namespace Eigen;

		Affine3f playerT = Affine3f::Identity();
		playerT.rotate(player->data.modelCore->GetWorldRotation());
		playerT.translation() = player->data.playerRigidbody->GetPosition();
		Vector3f playerPos = playerT.translation();

		Affine3f targetT = playerT;
		targetT.translate(targetPosition);

		Vector3f bodyPos = targetT.translation();
		// Vector3f bodyPos = selectBody->GetPosition();

		Vector3f playerToBody = bodyPos - playerPos;
		Vector3f direction = playerToBody.normalized();

		Vector3f to = (bodyPos - playerPos) / 2.f;
		Vector3f focus = playerPos + to;
		player->CameraLookAt(focus);
	}

	void AttachHoldState::CameraReset() const
	{
		player->ResetCameraCore();
	}

	void AttachHoldState::StateCancel() const
	{
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	void AttachHoldState::TrySelect()
	{
		attachble = player->data.holdObject;
		selectBody = player->data.holdObjectBody;

		if (!attachble || !selectBody)
		{
			player->ChangeAbilityState(Player::ATTACH_SELECT);
		}
		else
		{
			if (selectBody->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
				player->ChangeAbilityState(Player::ATTACH_SELECT);
			else
				VariableSet();
		}
	}

	void AttachHoldState::LookToWorldDirection(const Eigen::Vector3f& _to)
	{
#if _DEBUG
		debugVector1 = _to.normalized();
#endif _DEBUG

		player->LookInWorldDirection(_to.normalized());
	}

	void AttachHoldState::LookToLocalDirection(const Eigen::Vector3f& _to)
	{
#if _DEBUG
		debugVector2 = _to.normalized();
#endif _DEBUG

		player->LookInLocalDirection(_to.normalized());
	}

	void AttachHoldState::ApplyObjectVelocity() const
	{
		selectBody->SetLinearVelocity(targetVelocity + springL);
		selectBody->SetAngularVelocity(targetAngularVelocity + springR);
	}

	void AttachHoldState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();
	}

	//void AttachHoldState::CalculateSpringForces()
	//{
	//	constexpr float zeta = 0.5f;												// Damping ratio
	//	constexpr float zeta0 = 0.1f;												// Damping ratio
	//	constexpr float omega = 2.0f * std::numbers::pi_v<float> * 5.0f;			// Angular frequency (5 Hz)
	//	const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();	// Time step
	//
	//	{
	//		if (attachble)
	//		{
	//			using namespace Eigen;
	//
	//			const Vector3f pos = player->data.modelCore->GetWorldPosition();
	//			const Quaternionf rot = player->data.modelCore->GetWorldRotation();
	//			Affine3f transform = Affine3f::Identity();
	//			transform.translate(pos);
	//			transform.rotate(rot); 
	//			Affine3f invTrans = transform.inverse();
	//
	//			ZonaiPhysics::ZnBound3 bound = AttachSystem::Instance()->CalculateBoundingBox(attachble, invTrans.matrix());
	//			
	//			if (bound.minimum.z() < 0.5f)
	//			{
	//				float distance = 0.5f - bound.minimum.z();
	//
	//				targetPosition.z() += distance;
	//			}
	//		}
	//	}
	//
	//	{
	//		Eigen::Vector3f currPos = selectBody->GetPosition();
	//		currPos.y() = 0.f;				// stick R 입력으로 오브젝트를 움직일 때는 스프링이 아니므로..
	//
	//		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
	//		playerPos.y() = 0.f;
	//		Eigen::Vector3f zPos = player->data.modelCore->GetFront() * targetPosition.z();
	//		zPos.y() = 0.f;
	//		const Eigen::Vector3f targetP = playerPos + zPos;
	//
	//		posSpring.Update(
	//			currPos,
	//			springL,
	//			targetP,
	//			zeta,
	//			omega,
	//			timeStep
	//		);
	//	}
	//
	//	{
	//		Eigen::Quaternionf currRot = selectBody->GetRotation();
	//		const Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
	//		const Eigen::Quaternionf targetR = playerRot * targetRotation;
	//
	//		PurahEngine::GraphicsManager::GetInstance().DrawString(
	//			L"현재 회전 : " +
	//			std::to_wstring(currRot.w()) + L"\n" +
	//			std::to_wstring(currRot.x()) + L"\n" +
	//			std::to_wstring(currRot.y()) + L"\n" +
	//			std::to_wstring(currRot.z()) + L"\n",
	//			1200, 300,
	//			200, 600, 15,
	//			255, 255, 255, 255);
	//
	//		PurahEngine::GraphicsManager::GetInstance().DrawString(
	//			L"타겟 회전 : " +
	//			std::to_wstring(targetR.w()) + L"\n" +
	//			std::to_wstring(targetR.x()) + L"\n" +
	//			std::to_wstring(targetR.y()) + L"\n" +
	//			std::to_wstring(targetR.z()) + L"\n",
	//			1200, 500,
	//			200, 600, 15,
	//			255, 255, 255, 255);
	//
	//		using namespace Eigen;
	//		// Vector3f r = selectBody->GetAngularVelocity();
	//
	//		quatSpring.Update(
	//			currRot,
	//			springR,
	//			targetR.normalized(),
	//			zeta0,
	//			timeStep);
	//
	//		// selectBody->SetAngularVelocity(r);
	//		selectBody->SetRotation(currRot);
	//	}
	//
	//}

	void AttachHoldState::CalculateSpringForces()
	{
		CalculateSpringPosition();
		CalculateSpringRotation();
	}

	void AttachHoldState::CalculateSpringPosition()
	{
		constexpr float zeta = 0.5f;
		constexpr float omega = 2.0f * std::numbers::pi_v<float> *5.0f;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (attachble)
		{
			using namespace Eigen;

			const Vector3f pos = player->data.modelCore->GetWorldPosition();
			const Quaternionf rot = player->data.modelCore->GetWorldRotation();
			Affine3f transform = Affine3f::Identity();
			transform.translate(pos);
			transform.rotate(rot);
			Affine3f invTrans = transform.inverse();

			ZonaiPhysics::ZnBound3 bound = AttachSystem::Instance()->CalculateBoundingBox(attachble, invTrans.matrix());

			if (bound.minimum.z() < 0.5f)
			{
				float distance = 0.5f - bound.minimum.z();
				targetPosition.z() += distance;
			}
		}

		Eigen::Vector3f currPos = selectBody->GetPosition();
		currPos.y() = 0.f;

		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		playerPos.y() = 0.f;
		Eigen::Vector3f zPos = player->data.modelCore->GetFront() * targetPosition.z();
		zPos.y() = 0.f;
		const Eigen::Vector3f targetP = playerPos + zPos;

		posSpring.Update(currPos, springL, targetP, zeta, omega, timeStep);
	}

	void AttachHoldState::CalculateSpringRotation()
	{
		constexpr float zeta0 = 0.1f;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		Eigen::Quaternionf currRot = selectBody->GetRotation();
		const Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
		const Eigen::Quaternionf targetR = playerRot * targetRotation;

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"현재 회전 : " +
			std::to_wstring(currRot.w()) + L"\n" +
			std::to_wstring(currRot.x()) + L"\n" +
			std::to_wstring(currRot.y()) + L"\n" +
			std::to_wstring(currRot.z()) + L"\n",
			1200, 300, 200, 600, 15, 255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"타겟 회전 : " +
			std::to_wstring(targetR.w()) + L"\n" +
			std::to_wstring(targetR.x()) + L"\n" +
			std::to_wstring(targetR.y()) + L"\n" +
			std::to_wstring(targetR.z()) + L"\n",
			1200, 500, 200, 600, 15, 255, 255, 255, 255);

		quatSpring.Update(currRot, springR, targetR.normalized(), zeta0, timeStep);

		selectBody->SetRotation(currRot);
	}


	void AttachHoldState::TranslateSpringAlongY(float _distance)
	{
		targetPosition.y() += _distance;
	}

	void AttachHoldState::TranslateSpringAlongZ(float _distance)
	{
		targetPosition.z() += _distance;
	}

	void AttachHoldState::TranslateObject(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
	}

	void AttachHoldState::TranslateObjectAlongX(float _distance)
	{
		const Eigen::Vector3f playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const Eigen::Vector3f objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();
		const Eigen::Vector3f right = Eigen::Vector3f::UnitY().cross(forward);

		TranslateObject(right.normalized(), _distance);
	}

	void AttachHoldState::TranslateObjectAlongY(float _distance)
	{
		TranslateObject(Eigen::Vector3f::UnitY(), _distance);
	}

	void AttachHoldState::TranslateObjectAlongZ(float _distance)
	{
		const auto playerPos = player->GetGameObject()->GetTransform()->GetWorldPosition();
		const auto objPos = selectBody->GetPosition();
		Eigen::Vector3f forward = objPos - playerPos;
		forward.y() = 0.f;
		forward.normalize();

		TranslateObject(forward, _distance);
	}

	void AttachHoldState::RotateWithSpring(const Eigen::Vector3f& _axis, float _angle)
	{
		targetRotation = Eigen::Quaternionf(Eigen::AngleAxisf{ _angle, _axis }) * targetRotation;
	}

	void AttachHoldState::RotateSpringAlongX(float _angle)
	{
		Eigen::Vector3f dir = Eigen::Vector3f::UnitX();

		RotateWithSpring(dir, _angle);
	}

	void AttachHoldState::RotateSpringAlongY(float _angle)
	{
		const Eigen::Vector3f worldUp = Eigen::Vector3f::UnitY();

		RotateWithSpring(worldUp, _angle);
	}

	bool AttachHoldState::TryAttach() const
	{
		if (!attachble)
			return false;

		return AttachSystem::Instance()->TryAttach(attachble);
	}

	bool AttachHoldState::TryDettach() const
	{
		if (!attachble)
			return false;

		return AttachSystem::Instance()->Dettach(attachble);
	}

	void AttachHoldState::Put() const
	{
		if (!attachble)
			return;

		AttachSystem::Instance()->DeselectBody(attachble);
	}

	void AttachHoldState::EnableOutline(bool _value) const
	{
		if (!selectBody || !attachble)
			return;

		AttachSystem::Instance()->EnableOutline(attachble, _value, PurahColor::BlueViolet, PurahColor::BlueViolet);
	}

	void AttachHoldState::Snap()
	{
		using namespace Eigen;

		const Quaternionf objectRotation = selectBody->GetRotation();
		const Quaternionf playerRotation = player->data.modelCore->GetWorldRotation();

		const Vector3f objectPosition = selectBody->GetPosition();
		const Vector3f playerPosition = player->data.modelCore->GetWorldPosition();

		Vector3f playerToObject = objectPosition - playerPosition;
		playerToObject.y() = 0.f;
		playerToObject.normalize();

		// 오브젝트-플레이어 방향과 유사한 축을 찾음
		Quaternionf test = playerRotation.inverse() * objectRotation;
		Quaternionf front = FindAxis(test);
		targetRotation = front;

		targetRotation.normalize();
	}

	Eigen::Quaternionf AttachHoldState::FindAxis(const Eigen::Quaternionf& _direction)
	{
		using namespace Eigen;

		Quaternionf result;
		int size = axisies.size();

		float max = 0.f;
		for (int i = 0; i < size; i++)
		{
			float cosTheta = 0.f;
			const auto& [axis, data] = axisies[i];
			Quaternionf dir = axis.normalized();
			Quaternionf minus = Quaternionf(-dir.w(), -dir.x(), -dir.y(), -dir.z());

			cosTheta = _direction.dot(dir);									// 축마다 각도를 계산함
			cosTheta = std::clamp(cosTheta, -1.f, 1.f);

			if (max <= cosTheta)								// 가장 가까운 축을 찾음
			{
				if (result != dir)
				{
					result = dir;
					info = data;
					max = cosTheta;
				}
			}

			cosTheta = _direction.dot(minus);									// 축마다 각도를 계산함
			cosTheta = std::clamp(cosTheta, -1.f, 1.f);

			if (max <= cosTheta)								// 가장 가까운 축을 찾음
			{
				if (result != dir)
				{
					result = dir;
					info = data;
					max = cosTheta;
				}
			}
		}

 		return result;
	}

	void AttachHoldState::VariableSet()
	{
		using namespace Eigen;

		if (!attachble || !selectBody)
			return;

		AttachSystem::Instance()->SelectBody(attachble);
		EnableOutline(true);

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const Eigen::Vector3f playerPosition = player->data.modelCore->GetWorldPosition();

		Eigen::Vector3f lookTo = objectPosition - playerPosition;
		targetPosition.y() = lookTo.y();
		lookTo.y() = 0.f;
		LookToWorldDirection(lookTo);
		targetPosition.z() = lookTo.norm();

		info.info = None;

		Snap();

		auto p = player->data.cameraCore->GetLocalPosition();
		p -= Eigen::Vector3f{ 0.5f ,0.5f, 0.f };
		player->data.xAngle = 0.f;

		player->data.cameraCore->SetLocalPosition(p);

		//const Eigen::Quaternionf objectRotation = selectBody->GetRotation();
		//const Eigen::Quaternionf playerRotation = player->data.modelCore->GetWorldRotation();
		//targetRotation = playerRotation.inverse() * objectRotation;
	}

	void AttachHoldState::VariableReset()
	{
		using namespace Eigen;

		playerVelocity = Eigen::Vector3f::Zero();

		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();

		springL = Eigen::Vector3f::Zero();
		springR = Eigen::Vector3f::Zero();

		targetPosition = Eigen::Vector3f::Zero();
		targetRotation = Eigen::Quaternionf::Identity();

		diffWidth = -0.1f;
		selectBody = nullptr;
		attachble = nullptr;
	}

	bool AttachHoldState::TryTranslate(float _distance)
	{

		return true;
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
