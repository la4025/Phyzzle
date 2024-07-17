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

#pragma region Initialize
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
	}

	void AttachHoldState::VariableReset()
	{
		using namespace Eigen;

		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();

		linearSpringForce = Eigen::Vector3f::Zero();
		angularSpringForce = Eigen::Vector3f::Zero();

		targetPosition = Eigen::Vector3f::Zero();
		targetRotation = Eigen::Quaternionf::Identity();

		selectBody = nullptr;
		attachble = nullptr;
	}
#pragma endregion Initialize

#pragma region StateEvent
	void AttachHoldState::StateEnter()
	{
		using namespace Eigen;

		if (TrySelect())
		{
			Vector3f corePos = player->data.cameraCore->GetWorldPosition();
			Quaternionf coreRot = player->data.cameraCore->GetWorldRotation();

			player->data.cameraArm->SetLocalPosition(player->camData.armDefaultPosition);
			player->camData.armTargetPosition = player->camData.armDefaultPosition;
			
			auto rot = player->data.modelCore->GetLocalRotation();
			player->data.cameraArm->SetLocalRotation(rot);
			player->camData.armTargetRotation = rot;
			player->camData.xAngle = 0.f;

			player->data.cameraCore->SetWorldRotation(coreRot);
			player->data.cameraCore->SetWorldPosition(corePos);
		}
	}

	void AttachHoldState::StateExit()
	{
		{
			using namespace Eigen;
			player->SetCameraCoreLocalTargetPosition(player->camData.coreDefaultPosition);
			player->SetCameraCoreLocalTargetRotation(player->camData.coreDefaultRotation);
		}

		EnableOutline(false);
		VariableReset();
	}

	void AttachHoldState::StateStay()
	{
		// 타겟 포지션을 계산
		UpdateTargetPosition();

		// 스프링 힘을 계산
		CalculateSpringForces();

		// 오브젝트에 속력을 적용
		ApplyObjectVelocity();

		// 속력 리셋
		ResetObjectVelocity();

		if (player->data.debugMode)
		{
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

			SearchDebugDraw();
		}
	}

	void AttachHoldState::PostStateStay()
	{
		// 카메라 업데이트
		UpdateCamera();
	}

	void AttachHoldState::StateCancel()
	{
		EnableOutline(false);
		Put();
		Cancel();
	}
#pragma endregion StateEvent

#pragma region Input
	// 플레이어 이동
	void AttachHoldState::Stick_L()
	{
		PlayerMove(player->data.holdSpeed);

		// 플레이어 이동 방향과 같은 방향으로 힘을 줌
		Eigen::Vector3f playerLinearVelocity = player->data.playerRigidbody->GetLinearVelocity();
		TranslateObject(playerLinearVelocity, 1.f);
	}

	// 오브젝트 이동
	void AttachHoldState::Stick_R()
	{
		using namespace Eigen;

		// 키 입력이 없으면 힘 안줌
		if (player->currInput.Rstick.Size)
		{
			TranslateObjectAlongX(player->currInput.Rstick.X * player->currInput.Rstick.Size);
			TranslateObjectAlongY(player->currInput.Rstick.Y * player->currInput.Rstick.Size);
		}
	}

	void AttachHoldState::Trigger_L()
	{
		adjustmentMode = player->currInput.LTrigger;
	}

	// 취소
	void AttachHoldState::Click_A()
	{
		Put();
		Cancel();
	}

	// 부착
	void AttachHoldState::Click_B()
	{
		// 오브젝트를 이동시킬수 있는 상태라면
		// 붙일 수 있어야함.
		if (TryAttach())
		{
			Put();
			Cancel();
		}
	}

	// 취소
	void AttachHoldState::Click_X()
	{
		Put();
		Cancel();
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
		Cancel();
	}

	// 오브젝트 이동 및 회전
	void AttachHoldState::Click_DUp()
	{
		const bool justRotate = !adjustmentMode && roateMode;
		const bool justTranslate = !adjustmentMode && !roateMode;

		if (justTranslate)
		{
			TranslateSpringAlongZ(player->abilData.targetPositionZStep);
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
			TranslateSpringAlongZ(-player->abilData.targetPositionZStep);
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
		if (player->TryPlayerMove(_speed))
		{
			float x = player->currInput.Lstick.X;
			float y = player->currInput.Lstick.Y;

			bool front = y > 0.f;
			bool right = x > 0.f;
			bool xBigger = std::fabs(x) > std::fabs(y);

			if (front && !xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_FRONT);
			}
			else if (!front && !xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_BACK);
			}
			else if (right && xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_RIGHT);
			}
			else if (!right && xBigger)
			{
				player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_LEFT);
			}
		}
		else
		{
			player->ChangePlayerAnimationState(Player::PlayerState::ABILITY_IDLE);
		}
	}

#pragma region Camera
	void AttachHoldState::UpdateCamera()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();

		UpdateHoldingCameraPosition(localPosition, worldPosition);
		UpdateHoldingCameraRotation();

		if (player->ResolveCameraCollision(localPosition, worldPosition))
		{
			player->SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			player->SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void AttachHoldState::UpdateHoldingCameraPosition(Eigen::Vector3f& _local, Eigen::Vector3f& _world) const
	{
		using namespace Eigen;

		float offset = 1.f;

		constexpr float highMax = 10.f;
		constexpr float lowMax = -7.f;

		constexpr float farMin = 5.f;
		constexpr float farMax = 20.f;

		Vector3f lowCamPos[2] = 
		{
			player->camData.attachLowCamera0->GetLocalPosition(),
			player->camData.attachLowCamera1->GetLocalPosition()
		};

		Vector3f defaultCamPos[2] =
		{
			player->camData.attachDefaultCamera0->GetLocalPosition(),
			player->camData.attachDefaultCamera1->GetLocalPosition()
		};

		Vector3f highCamPos[2] =
		{
			player->camData.attachHighCamera0->GetLocalPosition(),
			player->camData.attachHighCamera1->GetLocalPosition()
		};

		Vector3f objPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();

		Vector3f direction = objPos - playerPos;
		const float dy = direction.y();
		direction.y() = 0.f;
		const float dz = direction.norm();

		Vector3f* activeCamPos = (dy > 0.f) ? highCamPos : lowCamPos;
		float totalY = (dy > 0.f) ? highMax : lowMax;

		float ratioY = std::clamp(std::fabs(dy) / std::fabs(totalY), 0.f, 1.f);
		float totalZ = farMax - farMin;
		float ratioZ = std::clamp(dz / totalZ, 0.f, 1.f);

		auto& math = PurahEngine::PurahMath::GetInstance();

		Vector3f interpolatedAF = player->lerp(activeCamPos[0], activeCamPos[1], ratioZ);
		Vector3f interpolatedDC = player->lerp(defaultCamPos[0], defaultCamPos[1], ratioZ);
		Vector3f finalInterpolatedCamera = player->lerp(interpolatedDC, interpolatedAF, ratioY);

		Affine3f world{ player->data.cameraArm->GetWorldMatrix() };
		world.translate(finalInterpolatedCamera);

		_local = finalInterpolatedCamera;
		_world = world.translation();
	}

	void AttachHoldState::UpdateHoldingCameraRotation() const
	{
		using namespace Eigen;

		Quaternionf modelRot = player->data.modelCore->GetLocalRotation();
		player->data.cameraArm->SetLocalRotation(modelRot);

		float offset = 1.f;

		constexpr float highMax = 10.f;
		constexpr float lowMax = -7.f;

		constexpr float farMin = 10.f;
		constexpr float farMax = 20.f;

		Quaternionf lowCamRot[2] =
		{
			player->camData.attachLowCamera0->GetLocalRotation(),
			player->camData.attachLowCamera1->GetLocalRotation()
		};

		Quaternionf defaultCamRot[2] =
		{
			player->camData.attachDefaultCamera0->GetLocalRotation(),
			player->camData.attachDefaultCamera1->GetLocalRotation()
		};

		Quaternionf highCamRot[2] =
		{
			player->camData.attachHighCamera0->GetLocalRotation(),
			player->camData.attachHighCamera1->GetLocalRotation()
		};

		Vector3f objPos = selectBody->GetPosition();
		Vector3f playerPos = player->data.modelCore->GetWorldPosition();

		Vector3f direction = objPos - playerPos;
		const float dy = direction.y();
		direction.y() = 0.f;
		const float dz = direction.norm();

		Quaternionf* activeCamRot = (dy > 0.f) ? highCamRot : lowCamRot;
		float totalY = (dy > 0.f) ? highMax : lowMax;

		float ratioY = std::clamp(std::fabs(dy) / std::fabs(totalY), 0.f, 1.f);
		float totalZ = farMax - farMin;
		float ratioZ = std::clamp(dz / totalZ, 0.f, 1.f);

		auto& math = PurahEngine::PurahMath::GetInstance();

		Quaternionf interpolatedAF = activeCamRot[0].slerp(ratioZ, activeCamRot[1]);
		Quaternionf interpolatedDC = defaultCamRot[0].slerp(ratioZ, defaultCamRot[1]);
		Quaternionf finalInterpolatedCamera = interpolatedDC.slerp(ratioY, interpolatedAF);

		player->SetCameraCoreLocalTargetRotation(finalInterpolatedCamera);
	}

	void AttachHoldState::CameraReset() const
	{
		player->ResetCameraCore();
	}
#pragma endregion Camera

	void AttachHoldState::Cancel() const
	{
		player->ChangeAbilityState(Player::AbilityState::DEFAULT);
	}

	bool AttachHoldState::TrySelect()
	{
		attachble = player->data.holdObject;
		selectBody = player->data.holdObjectBody;

		if (!attachble || !selectBody)
		{
			player->ChangeAbilityState(Player::ATTACH_SELECT);
			return false;;
		}
		else
		{
			if (selectBody->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
			{
				player->ChangeAbilityState(Player::ATTACH_SELECT);
				return false;;
			}
			else
			{
				VariableSet();
				return true;
			}
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
		using namespace Eigen;

		//selectBody->SetLinearVelocity(targetVelocity + springL);
		//selectBody->SetAngularVelocity(targetAngularVelocity + springR);

		constexpr float dampingFactor = 3.f;

		Vector3f currentLinearVelocity = selectBody->GetLinearVelocity();
		// Vector3f dampingForce = Vector3f::Zero();
		Vector3f dampingForce = -dampingFactor * currentLinearVelocity;
		Vector3f targetLinearVelocity = targetVelocity + linearSpringForce;
		Vector3f additionalLinearVelocity = targetLinearVelocity - currentLinearVelocity;
		additionalLinearVelocity = additionalLinearVelocity + dampingForce;
		selectBody->AddForce(additionalLinearVelocity, ZonaiPhysics::Velocity_Change);

		Vector3f currentAngularVelocity = selectBody->GetAngularVelocity();
		// Vector3f dampingTorque = Vector3f::Zero();
		Vector3f dampingTorque = -dampingFactor * currentAngularVelocity;
		Vector3f targetAngularVelocity0 = targetAngularVelocity + angularSpringForce;
		Vector3f additionalAngularVelocity = targetAngularVelocity0 + dampingTorque;
		selectBody->AddTorque(additionalAngularVelocity, ZonaiPhysics::Velocity_Change);
	}

	void AttachHoldState::ResetObjectVelocity()
	{
		targetVelocity = Eigen::Vector3f::Zero();
		targetAngularVelocity = Eigen::Vector3f::Zero();
	}

	// void AttachHoldState::SpringMassModel(const Eigen::Vector3f& worldTargetPosition)
	// {
	// 	using namespace Eigen;
	// 
	// 	constexpr float springConstant = 10.f;
	// 	constexpr float dampingFactor = 3.f;
	// 
	// 	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();
	// 
	// 	// 현재 오브젝트 위치
	// 	Eigen::Vector3f currentPosition = selectBody->GetPosition();
	// 
	// 	// 스프링 힘 계산 (후크의 법칙)
	// 	Eigen::Vector3f displacement = worldTargetPosition - currentPosition;
	// 	Eigen::Vector3f springForce = springConstant * displacement;
	// 
	// 	// Rigidbody의 속도 가져오기
	// 	Eigen::Vector3f objectVelocity = selectBody->GetLinearVelocity();
	// 
	// 	// 감쇠 힘 계산
	// 	Eigen::Vector3f dampingForce = -dampingFactor * objectVelocity;
	// 
	// 	// 총 힘 계산
	// 	Eigen::Vector3f totalForce = springForce + dampingForce;
	// 
	// 	// 오브젝트에 힘 적용
	// 	selectBody->AddForce(totalForce);
	// }

	//void AttachHoldState::UpdateTargetPosition()
	//{
	//	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();
	//
	//	// 현재 오브젝트 위치
	//	Eigen::Vector3f currentPosition = selectBody->GetPosition();
	//
	//	// 월드 좌표에서의 타겟 포지션을 가져옵니다.
	//	Eigen::Vector3f worldTargetPosition = GetWorldTargetPosition();
	//
	//	// 타겟 포지션이 오브젝트를 향해 이동
	//	Eigen::Vector3f directionToObject = (currentPosition - worldTargetPosition).normalized();
	//	worldTargetPosition += directionToObject * 1.f * deltaTime;
	//
	//	Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
	//	Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();
	//
	//	targetPosition = playerRot.inverse() * (worldTargetPosition - playerPos);
	//}

	Eigen::Vector3f AttachHoldState::GetWorldTargetPosition()
	{
		// 플레이어의 월드 위치와 회전
		Eigen::Vector3f playerPos = player->data.modelCore->GetWorldPosition();
		Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();

		// 로컬 타겟 포지션을 월드 좌표로 변환
		return playerPos + playerRot * targetPosition;
	}

	Eigen::Quaternionf AttachHoldState::GetWorldTargetQuaternion()
	{
		// 플레이어의 월드 회전
		Eigen::Quaternionf playerRot = player->data.modelCore->GetWorldRotation();

		// 로컬 타겟 회전을 월드 회전으로 변환
		return playerRot * targetRotation;
	}

	void AttachHoldState::UpdateTargetPosition()
	{
		using namespace Eigen;

		if (attachble)
		{
			Affine3f targetTransform = Affine3f::Identity();
			targetTransform.translate(targetPosition);
			targetTransform.rotate(targetRotation);

			ZonaiPhysics::ZnBound3 bound = AttachSystem::Instance()->ComputeBoundingBoxAtTransform(attachble, targetTransform.matrix());

			// 바운딩 박스의 최소 z 값이 1.0보다 작으면 targetPosition.z를 업데이트
			if (bound.minimum.z() < player->abilData.minTargetPositionZ)
			{
				float distance = player->abilData.minTargetPositionZ - bound.minimum.z();
				targetPosition.z() += distance;
			}

			// targetPosition.z가 max를 넘으면 조정
			if (targetPosition.z() > player->abilData.maxTargetPositionZ)
			{
				float distance = player->abilData.maxTargetPositionZ - targetPosition.z();
				targetPosition.z() += distance;
			}

			// targetPosition.y가 min를 넘으면 조정
			if (targetPosition.y() < player->abilData.minTargetPositionY)
			{
				float distance = player->abilData.minTargetPositionY - targetPosition.y();
				targetPosition.y() += distance;
			}

			// targetPosition.y가 max를 넘으면 조정
			if (targetPosition.y() > player->abilData.maxTargetPositionY)
			{
				float distance = player->abilData.maxTargetPositionY - targetPosition.y();
				targetPosition.y() += distance;
			}
		}
	}

	void AttachHoldState::CalculateSpringForces()
	{
		CalculateSpringPosition();
		CalculateSpringRotation();
	}

	void AttachHoldState::CalculateSpringPosition()
	{
		using namespace Eigen;
		
		const float zeta = player->abilData.linearSpringDamping;
		const float omega = player->abilData.linearSpringFrequency;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		UpdateTargetPosition();

		Eigen::Vector3f currPos = selectBody->GetPosition();
		Eigen::Vector3f worldTargetPosition = GetWorldTargetPosition();

		//float distanceToTarget = (worldTargetPosition - currPos).norm();
		//float adjustedZeta = zeta + distanceToTarget; // 목표 위치에 가까워질수록 스프링 상수 증가

		posSpring.UpdateVelocity(currPos, linearSpringForce, worldTargetPosition, zeta, omega, timeStep);

		//{
		//	linearSpringForce = (worldTargetPosition - currPos) / 0.01f;
		//}

		if (linearSpringForce.norm() > player->abilData.linearMaxVelocity)
		{
			linearSpringForce = linearSpringForce.normalized() * player->abilData.linearMaxVelocity;
		}
	}

	void AttachHoldState::CalculateSpringRotation()
	{
		// constexpr float zeta0 = 0.05f;
		const float zeta = player->abilData.angularSpringDamping;
		const float omega = player->abilData.angularSpringFrequency;
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		Eigen::Quaternionf currRot = selectBody->GetRotation();
		Eigen::Quaternionf targetRot = GetWorldTargetQuaternion();

		//float angleToTarget = currRot.angularDistance(targetRot);
		//float adjustedZeta = zeta + angleToTarget; // 목표 회전에 가까워질수록 스프링 상수 증가

		if (player->data.debugMode)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"현재 위치 : " +
				std::to_wstring(currRot.x()) + L"\n" +
				std::to_wstring(currRot.y()) + L"\n" +
				std::to_wstring(currRot.z()) + L"\n",
				1200, 300, 200, 600, 15, 255, 255, 255, 255);

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"목표 위치 : " +
				std::to_wstring(targetRot.x()) + L"\n" +
				std::to_wstring(targetRot.y()) + L"\n" +
				std::to_wstring(targetRot.z()) + L"\n",
				1200, 500, 200, 600, 15, 255, 255, 255, 255);
		}

		//{
		//	// Adjust the target angle to the shortest path
		//	Eigen::Quaternionf goal = targetRot;
		//	Eigen::Quaternionf minusGoal = { -targetRot.w(), -targetRot.x(), -targetRot.y(), -targetRot.z() };

		//	float xDot = currRot.dot(goal);
		//	float minusDot = currRot.dot(minusGoal);

		//	if (xDot < minusDot)
		//		goal = minusGoal;

		//	// Compute the relative quaternion from current to target
		//	Eigen::Quaternionf q_rel = goal * currRot.conjugate();
		//	q_rel.normalize();

		//	// Extract the vector part of the relative quaternion (imaginary part)
		//	Eigen::Vector3f relative_angle_axis = q_rel.vec();
		//	
		//	angularSpringForce = relative_angle_axis / 0.01f;
		//}

		quatSpring.UpdateVelocity(currRot, angularSpringForce, targetRot.normalized(), zeta, omega, timeStep);

		if (angularSpringForce.norm() > player->abilData.angularMaxVelocity)
		{
			angularSpringForce = angularSpringForce.normalized() * player->abilData.angularMaxVelocity;
		}
	}

	void AttachHoldState::TranslateSpringAlongY(float _distance)
	{
		targetPosition.y() += _distance;
	}

	void AttachHoldState::TranslateSpringAlongZ(float _distance)
	{
		using namespace Eigen;

		targetPosition.z() += _distance;
	}

	void AttachHoldState::TranslateObjectAlongX(float _factor)
	{
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// 최대 회전 각도
		const float angleAtRadiusOne = player->abilData.holdRotateAngle;

		// 반지름이 1일 때의 호의 길이 계산
		const float arcLength = 1.0f * angleAtRadiusOne;

		// targetPosition.z의 길이로 회전 각도 계산
		const float targetRadius = targetPosition.z();
		const float angle = (arcLength / targetRadius) * player->abilData.arcRatio;
		const float finalAngle = angle * _factor * timeStep;

		const Eigen::Vector3f axis = Eigen::Vector3f::UnitY();

		// 플레이어의 월드 위치와 회전
		const Eigen::Vector3f playerPosition = player->data.modelCore->GetWorldPosition();
		Eigen::Quaternionf playerRotation = player->data.modelCore->GetWorldRotation();
		playerRotation = Eigen::AngleAxisf(finalAngle * (std::numbers::pi / 180.f), axis) * playerRotation;
		const Eigen::Vector3f newTargetPosition = playerPosition + playerRotation * targetPosition;

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const float distance = (newTargetPosition - objectPosition).norm();
		if (distance <= player->abilData.targetPositionOffset)
		{
			player->data.modelCore->SetWorldRotation(playerRotation);
		}
	}

	void AttachHoldState::TranslateObjectAlongY(float _factor)
	{
		const float timeStep = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		Eigen::Vector3f newTargetPosition = targetPosition;
		newTargetPosition.y() += (player->abilData.targetPositionYSpeed * _factor * timeStep);

		// 플레이어의 월드 위치와 회전
		const Eigen::Vector3f playerPosition = player->data.modelCore->GetWorldPosition();
		const Eigen::Quaternionf playerRotation = player->data.modelCore->GetWorldRotation();
		const Eigen::Vector3f worldTargetPosition = playerPosition + playerRotation * newTargetPosition;

		const Eigen::Vector3f objectPosition = selectBody->GetPosition();
		const float distance = (worldTargetPosition - objectPosition).norm();
		if (distance <= player->abilData.targetPositionOffset)
		{
			targetPosition = newTargetPosition;
		}
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

	// 타겟 속력을 업데이트가 아닌
	void AttachHoldState::TranslateObject(const Eigen::Vector3f& _direction, float power)
	{
		targetVelocity += (_direction * power);
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

	void AttachHoldState::RotateWithSpring(const Eigen::Vector3f& _axis, float _angle)
	{
		targetRotation = Eigen::Quaternionf(Eigen::AngleAxisf{ _angle, _axis }) * targetRotation;
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

	void AttachHoldState::EnableOutline(bool _value) const
	{
		if (!selectBody || !attachble)
			return;

		if (_value)
		{
			AttachSystem::Instance()->EnableOutline(attachble, &player->color0, &player->color1);
			AttachSystem::Instance()->EnableDShadow(attachble);
		}
		else
		{
			AttachSystem::Instance()->DisableOutline(attachble);
			AttachSystem::Instance()->DisableDShadow(attachble);
		}
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

	void AttachHoldState::Put() const
	{
		if (!attachble)
			return;

		AttachSystem::Instance()->DeselectBody(attachble);
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

	bool AttachHoldState::TryTranslate(float _distance)
	{

		return true;
	}
#pragma endregion Content

#pragma region Debug
	void AttachHoldState::SearchDebugDraw()
	{
		//PurahEngine::GraphicsManager::GetInstance().DrawString(
		//	L"player to object : " +
		//	std::to_wstring(debugVector0.x()) + L" " +
		//	std::to_wstring(debugVector0.y()) + L" " +
		//	std::to_wstring(debugVector0.z()) + L" ",
		//	1200, 100,
		//	200, 600, 15,
		//	255, 255, 255, 255);

		//PurahEngine::GraphicsManager::GetInstance().DrawString(
		//	L"world direction : " +
		//	std::to_wstring(debugVector1.x()) + L" " +
		//	std::to_wstring(debugVector1.y()) + L" " +
		//	std::to_wstring(debugVector1.z()) + L" ",
		//	1200, 200,
		//	200, 600, 15,
		//	255, 255, 255, 255);

		//PurahEngine::GraphicsManager::GetInstance().DrawString(
		//	L"local direction : " +
		//	std::to_wstring(debugVector2.x()) + L" " +
		//	std::to_wstring(debugVector2.y()) + L" " +
		//	std::to_wstring(debugVector2.z()) + L" ",
		//	1200, 300,
		//	200, 600, 15,
		//	255, 255, 255, 255);

		using namespace Eigen;

		Vector3f linearVelo = selectBody->GetLinearVelocity();
		float velo = linearVelo.norm();
		Vector3f angularVelo = selectBody->GetAngularVelocity();
		float aVelo = angularVelo.norm();

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"object velocity : " +
			std::to_wstring(velo),
			1200, 100,
			200, 600, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"object angularVelocity : " +
			std::to_wstring(aVelo),
			1200, 200,
			200, 600, 15,
			255, 255, 255, 255);
	}
#pragma endregion Debug
}
