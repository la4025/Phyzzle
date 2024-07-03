#include <algorithm>
#include <iostream>

#include "IState.h"
#include "DefaultState.h"
#include "AttachHoldState.h"
#include "AttachSelectState.h"
#include "RewindState.h"
#include "LockState.h"
#include "PzObject.h"
#include "Rewindable.h"

#include "Player.h"

#include "../GraphicsTest/CoreSystem.h"

namespace Phyzzle
{
	Player::~Player()
	{
		for (auto& [key, State] : stateSystem)
		{
			delete State;
			State = nullptr;
		}
	}

	void Player::InitializeGamePad()
	{
		gamePad = PurahEngine::GamePadManager::GetGamePad(0);
	}

	void Player::InitializeDefaultPositions()
	{
		data.armDefaultPosition = data.cameraArm->GetLocalPosition();
		data.armDefaultRotation = data.cameraArm->GetLocalRotation();

		data.coreDefaultPosition = data.cameraCore->GetLocalPosition();
		data.coreDefaultRotation = data.cameraCore->GetLocalRotation();

		data.coreSelectPosition = data.coreDefaultPosition + Eigen::Vector3f{0.5f, 0.5f, 0.f};

		highPosition = Eigen::Vector3f(0.f, 0.f, -20.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - data.coreDefaultPosition;
		differenceLow = lowPosition - data.coreDefaultPosition;
	}

	void Player::InitializeLerpFunctions()
	{
		lerp = [this](const Eigen::Vector3f start, const Eigen::Vector3f end, float _t) -> Eigen::Vector3f
			{
				if (start == end)
					return start;

				_t = std::clamp(_t, 0.f, 1.f);

				return start + _t * (end - start);
			};

		slerp = [this](const Eigen::Quaternionf start, const Eigen::Quaternionf end, float _t) -> Eigen::Quaternionf
			{
				if (start == end)
					return start;

				_t = std::clamp(_t, 0.f, 1.f);

				return start.slerp(_t, end);
			};
	}

	void Player::InitializeAbilitySystem()
	{
		stateSystem.insert(std::make_pair(ATTACH_HOLD, new AttachHoldState(this)));
		stateSystem.insert(std::make_pair(DEFAULT, new DefaultState(this)));
		stateSystem.insert(std::make_pair(ATTACH_SELECT, new AttachSelectState(this)));
		stateSystem.insert(std::make_pair(REWIND_SELECT, new RewindState(this)));
		stateSystem.insert(std::make_pair(LOCK_SELECT, new LockState(this)));

		stateChange.insert(ATTACH_SELECT);
		stateChange.insert(REWIND_SELECT);
		stateChange.insert(LOCK_SELECT);
	}

	void Player::InitializeStateSystem()
	{
		animationState.insert(std::make_pair(IDLE, 
			[this]() 
			{
				data.animator->Play(data.idleAnimation);
			}
		));
		animationState.insert(std::make_pair(WALK,
			[this]()
			{
				data.animator->Play(data.runningAnimation);
			}
		));
		animationSpeedController.insert(std::make_pair(WALK,
			[this]()
			{
				data.animator->SetPlaySpeed(data.runningAnimation, currInput.Lstick.Size);
			}
		));
		animationState.insert(std::make_pair(JUMP,
			[this]()
			{
				data.animator->Play(data.jumpAnimation);
			}
		));
		animationState.insert(std::make_pair(JUMPING,
			[this]()
			{
				data.animator->Play(data.jumpingAnimation);
			}
		));
		animationState.insert(std::make_pair(LANDING,
			[this]()
			{
				data.animator->Play(data.landingAnimation);
			}
		));
		animationState.insert(std::make_pair(ABILITY_IDLE,
			[this]()
			{
				data.animator->Play(data.holdIdleAnimation);
			}
		));
		animationState.insert(std::make_pair(ABILITY_FRONT,
			[this]()
			{
				data.animator->Play(data.holdFrontAnimation);
			}
		));
		animationSpeedController.insert(std::make_pair(ABILITY_FRONT,
			[this]()
			{
				data.animator->SetPlaySpeed(data.holdFrontAnimation, currInput.Lstick.Size);
			}
		));
		animationState.insert(std::make_pair(ABILITY_BACK,
			[this]()
			{
				data.animator->Play(data.holdBackAnimation);
			}
		));
		animationSpeedController.insert(std::make_pair(ABILITY_BACK,
			[this]()
			{
				data.animator->SetPlaySpeed(data.holdBackAnimation, currInput.Lstick.Size);
			}
		));
		animationState.insert(std::make_pair(ABILITY_RIGHT,
			[this]()
			{
				data.animator->Play(data.holdRightAnimation);
			}
		));
		animationSpeedController.insert(std::make_pair(ABILITY_RIGHT,
			[this]()
			{
				data.animator->SetPlaySpeed(data.holdRightAnimation, currInput.Lstick.Size);
			}
		));
		animationState.insert(std::make_pair(ABILITY_LEFT,
			[this]()
			{
				data.animator->Play(data.holdLeftAnimation);
			}
		));
		animationSpeedController.insert(std::make_pair(ABILITY_LEFT,
			[this]()
			{
				data.animator->SetPlaySpeed(data.holdLeftAnimation, currInput.Lstick.Size);
			}
		));
	}

#pragma region Event
	void Player::Start()
	{
		InitializeGamePad();
		InitializeDefaultPositions();
		InitializeAbilitySystem();
		InitializeStateSystem();
		InitializeLerpFunctions();
	}

	void Player::Update()
	{

		prevState = currState;
		prevPlayerState = currPlayerState;
		
		if (!data.stopUpdate)
		{
			HandleGamePadInput();
		}
		
		DebugDraw();
		UpdateAbilityState();
		UpdatePlayerState();
	}

	void Player::LateUpdate()
	{
		UpdateDefaultCamera();
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		JumpCheck(zn_collision, collider);
	}

	void Player::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		JumpCheck(zn_collision, collider);
	}

	void Player::SetStopUpdate(bool _value)
	{
		data.stopUpdate = _value;
	}

#pragma endregion Event
	void Player::DebugDraw()
	{
		DrawStateInfo();
		DrawJumpInfo();
	}

	void Player::DrawStateInfo() const
	{
		std::wstring str = GetStateString(data.state);
		std::wstring str0 = GetStateString(currState);

		PurahEngine::GraphicsManager::GetInstance().DrawString(L"SELETE STATE : " + str, 100, 100, 300, 100, 15, 255, 255, 255, 255);
		PurahEngine::GraphicsManager::GetInstance().DrawString(L"PLAYER STATE : " + str0, 100, 200, 400, 100, 15, 255, 255, 255, 255);
	}

	void Player::UpdateAbilityState()
	{
		if (prevState != currState)
		{
			stateSystem[prevState]->StateExit();
			stateSystem[currState]->StateEnter();
		}
		else
		{
			stateSystem[currState]->StateStay();
		}
	}

	void Player::UpdatePlayerState()
	{
		if (prevPlayerState != currPlayerState)
		{
			animationState[currPlayerState]();
		}
		else
		{
			if (animationSpeedController.contains(currPlayerState))
			{
				animationSpeedController[currPlayerState]();
			}
		}
	}

	std::wstring Player::GetStateString(AbilityState state) const
	{
		switch (state)
		{
		case ATTACH_HOLD: return L"ATTACH_HOLD";
		case ATTACH_SELECT: return L"ATTACH_SELECT";
		case REWIND_SELECT: return L"REWIND_SELECT";
		case LOCK_SELECT: return L"LOCK_SELECT";
		default: return L"DEFAULT";
		}
	}

	void Player::DrawJumpInfo() const
	{
		std::wstring jumpStatus = data.jumping ? L"Jumping" : L"can Jump";
		PurahEngine::GraphicsManager::GetInstance().DrawString(jumpStatus, 500, 200, 400, 200, 15, 255, 255, 255, 255);
	}

	void Player::HandleGamePadInput()
	{
		if (gamePad->IsConnected())
		{
			HandleStickInput();
			HandleTriggerInput();
			HandleButtonInput();
		}
	}

	void Player::HandleStickInput()
	{
		currInput.Lstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, currInput.Lstick.X, currInput.Lstick.Y);
		currInput.Rstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, currInput.Rstick.X, currInput.Rstick.Y);

		stateSystem[currState]->Stick_L();
		stateSystem[currState]->Stick_R();
	}

	void Player::HandleTriggerInput()
	{
		currInput.LTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
		currInput.RTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

		stateSystem[currState]->Trigger_L();
		stateSystem[currState]->Trigger_R();
	}

	void Player::HandleButtonInput()
	{
		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_L, &IState::Click_LB, &IState::Pressing_LB, &IState::Up_LB);
		HandleButton(PurahEngine::ePad::ePAD_SHOULDER_R, &IState::Click_RB, &IState::Pressing_RB, &IState::Up_RB);
		HandleButton(PurahEngine::ePad::ePAD_A, &IState::Click_A, &IState::Pressing_A, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_B, &IState::Click_B, &IState::Pressing_B, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_X, &IState::Click_X, &IState::Pressing_X, &IState::Up_X);
		HandleButton(PurahEngine::ePad::ePAD_Y, &IState::Click_Y, &IState::Pressing_Y, &IState::Up_Y);
		HandleButton(PurahEngine::ePad::ePAD_UP, &IState::Click_DUp, &IState::Pressing_DUp, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_DOWN, &IState::Click_DDown, &IState::Pressing_DDown, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_LEFT, &IState::Click_DLeft, &IState::Pressing_DLeft, nullptr);
		HandleButton(PurahEngine::ePad::ePAD_RIGHT, &IState::Click_DRight, &IState::Pressing_DRight, nullptr);
	}

	void Player::HandleButton(PurahEngine::ePad button, void (IState::* clickFunc)(), void (IState::* pressingFunc)(), void (IState::* upFunc)())
	{
		if (gamePad->IsKeyDown(button))
		{
			(stateSystem[currState]->*clickFunc)();
		}
		else if (gamePad->IsKeyPressed(button))
		{
			if (pressingFunc)
			{
				(stateSystem[currState]->*pressingFunc)();
			}
		}
		else if (gamePad->IsKeyUp(button))
		{
			if (upFunc)
			{
				(stateSystem[currState]->*upFunc)();
			}
		}
	}

	bool Player::TryJump()
	{
		if (data.jumping)
		{
			return false;
		}

		Eigen::Vector3f power = Eigen::Vector3f::UnitY() * data.jumpPower;
		data.playerRigidbody->AddForce(power, ZonaiPhysics::ForceType::Accelration);
		data.jumping = true;

		return true;
	}

	void Player::JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		for (size_t i = 0; i < zn_collision.contactCount; i++)
		{
			Eigen::Vector3f velo = data.playerRigidbody->GetLinearVelocity();
			velo.x() = 0.f;
			velo.z() = 0.f;
			if (velo.squaredNorm() > 0.f)
			{
				velo.normalize();
			}
			auto normal = zn_collision.contacts[i].normal;

			// 플레이어 속도랑 충돌 노말 방향이랑 다른 방향인지 체크
			if (!IsOppositeDirection(velo, normal))
			{
				return;
			}

			// up 벡터랑 노말과 각도가 
			if (IsGrounded(normal))
			{
				data.jumping = false;
				return;
			}
		}
	}

	bool Player::IsOppositeDirection(const Eigen::Vector3f& velo, const Eigen::Vector3f& normal) const
	{
		const float cosTheta0 = velo.dot(normal);

		return cosTheta0 < 0;
	}

	bool Player::IsGrounded(const Eigen::Vector3f& normal) const
	{
		// up 벡터랑 노말과 각도가 혀용 범위 내인가?
		const Eigen::Vector3f up{ 0.f, 1.f, 0.f };
		float cosTheta = up.dot(normal);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float slope = std::acosf(cosTheta);
		const float slopeAngleLimit = (data.slopeLimit * std::numbers::phi_v<float>) / 180.f;

		return slope <= slopeAngleLimit;
	}

	bool Player::TryPlayerMove(float _moveSpeed)
	{
		// 카메라의 전방 벡터를 계산
		const Eigen::Vector3f cameraFront = data.cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = data.cameraArm->GetRight();

		// 속도 벡터를 계산
		const Eigen::Vector3f movementDirection = forward * currInput.Lstick.Y + right * currInput.Lstick.X;
		const Eigen::Vector3f movement = movementDirection * _moveSpeed * currInput.Lstick.Size;

		Eigen::Vector3f velocity = data.playerRigidbody->GetLinearVelocity();

		// y축 운동을 방해하지 않는 걸로 중력은 적용되도록함.
		/// 단, 이런 방식이면 키 입력이 없다면 누군가랑 부딪쳐도 가만히 있을 듯
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// 속력을 적용시킴
		// data.playerRigidbody->AddForce(velocity, ZonaiPhysics::Velocity_Change);
		data.playerRigidbody->SetLinearVelocity(velocity);

		return currInput.Lstick.Size >= 1e-6;
	}

	/// \brief 모델을 월드 방향 벡터를 향해 회전
	void Player::LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const
	{
		if (_worldDirection.isZero())
			return;

		// 플레이어의 월드 회전을 구함
		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();
		// Model의 로컬 방향을 구함.
		const Eigen::Vector3f localDirection = parentWorld.conjugate() * _worldDirection;

		LookInLocalDirection(localDirection);
	}

	/// \brief 모델을 Player의 로컬 방향 벡터를 향해 회전
	void Player::LookInLocalDirection(const Eigen::Vector3f& _localDirection) const
	{
		if (_localDirection.isZero())
			return;

		// 로컬 Z와 Local Direction 사이의 쿼터니언을 구함.
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), _localDirection);

		// Model을 회전시킴
		data.modelCore->SetLocalRotation(targetRotation);
	}

#pragma region camera
	void Player::UpdateDefaultCamera()
	{
		RotateCameraArm();
		UpdateDefaultCameraCore();

		if (data.isTransitioning)
		{

		}
		else
		{

		}

	}

	void Player::UpdateSelectCamera()
	{

	}

	void Player::UpdateHoldCamera()
	{

	}

	// default 상태에서 camera core의 위치를 계산하는 함수 
	void Player::UpdateDefaultCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition);
		
		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
		
		UpdateCameraLerp();
		CharacterDisable();
	}

	void Player::CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut)
	{
		using namespace Eigen;

		const float ease = data.xAngle >= 0.f ? data.xAngle / data.limitHighAngle : data.xAngle / data.limitLowAngle;
		auto easingFunc = data.xAngle >= 0.f ?
			[](float x) { return 1.0f - (1.0f - x) * (1.0f - x); } :
			[](float x) { return 1.0f - std::pow(1.0f - x, 5.0f); };

		const Vector3f& diff = data.xAngle >= 0.f ? differenceHigh : differenceLow;

		Vector3f pos = data.coreTargetPosition;
		pos.z() = data.coreDefaultPosition.z() + diff.z() * easingFunc(ease);

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		world.translate(pos);

		localOut = pos;
		worldOut = world.translation();
	}

	bool Player::ResolveCameraCollision(Eigen::Vector3f& localIn, Eigen::Vector3f& worldIn)
	{
		using namespace Eigen;

		static float prevDistance = FLT_MAX;

		float radius = data.cameraCollisionRadius;
		const Vector3f coreLocalPos = localIn;
		const Vector3f coreWorldPos = worldIn;

		const Vector3f coreLocalDir = data.cameraCore->GetLocalRotation() * Vector3f::UnitZ();
		const Vector3f localStart = CalculateCameraFocusPosition(coreLocalPos, coreLocalDir);

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		world.translate(localStart);
		const Vector3f worldStart = world.translation();

		Vector3f worldDir = coreWorldPos - worldStart;
		const float dis = worldDir.norm();
		worldDir.normalize();

		unsigned int layers = data.cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;
		ZonaiPhysics::ZnQueryInfo info2;

		bool hit = PurahEngine::Physics::Spherecast(radius, worldStart, Quaternionf::Identity(), worldDir, dis, layers, info);
		bool hit2 = PurahEngine::Physics::Spherecast(radius + 0.2f, worldStart, Quaternionf::Identity(), worldDir, dis, layers, info2);

		if (!hit)
		{
			return hit;
		}

		Vector3f position = worldStart + worldDir * info.distance;
		Affine3f worldT{ data.cameraArm->GetWorldMatrix() };
		Vector3f localPos = worldT.inverse() * position;

		// position = worldStart + worldDir * info.distance;
		// worldT = data.cameraArm->GetWorldMatrix();
		// localPos = worldT.inverse() * position;

		if (info.distance < prevDistance)
		{
			data.cameraCore->SetLocalPosition(localPos);
		}

		//else if (hit2)
		//{
		//	position = worldStart + worldDir * info2.distance;
		//	worldT = data.cameraArm->GetWorldMatrix();
		//	localPos = worldT.inverse() * position;
		//	
		//	prevDistance = info2.distance;
		//}

		data.coreTargetPosition = localPos;

		localIn = localPos;
		worldIn = position;

		prevDistance = info.distance;

		return hit;
	}

	void Player::UpdateCameraLerp()
	{
		using namespace Eigen;
		
		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Vector3f currPos = data.cameraCore->GetLocalPosition();
		Vector3f target = data.coreTargetPosition;

		float inv = dt / data.cameraLerpTime;

		Vector3f pos = lerp(currPos, target, inv);

		data.cameraCore->SetLocalPosition(pos);
	}

	void Player::CharacterDisable()
	{
		using namespace Eigen;
		
		Vector3f vec = data.cameraCore->GetLocalPosition();
		float distance = vec.norm();
		PurahEngine::GameObject* obj = data.modelCore->GetGameObject();

		if (distance < 3.f)
		{
			obj->SetEnable(false);
		}
		else
		{
			obj->SetEnable(true);
		}
	}

	void Player::SetCameraCoreLocalTargetPosition(const Eigen::Vector3f& _localPosision)
	{
		// data.coreCurrentPosition = data.cameraCore->GetLocalPosition();
		data.coreTargetPosition = _localPosision;
	}

	void Player::SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision)
	{
		using namespace Eigen;

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		Vector3f localPos = world.inverse() * _worldPosision;
		
		// data.coreCurrentPosition = data.cameraCore->GetLocalPosition();
		data.coreTargetPosition = localPos;
	}

	Eigen::Vector3f Player::CalculateCameraFocusPosition(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction)
	{
		using namespace Eigen;

		Vector3f xy;
		Vector3f xz;
		bool hasXY = IntersectXYPlane(cameraPos, direction, xy);
		bool hasXZ = IntersectXZPlane(cameraPos, direction, xz);

		if (hasXY && hasXZ)
		{
			if (xy.z() < xz.z())
			{
				return xy;
			}
			else
			{
				return xz;
			}
		}
		else if (hasXY)
		{
			return xy;
		}
		else
		{
			return xz;
		}
	}

	bool Player::IntersectXYPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.z()) < 1e-6) 
		{
			return false;
		}

		float t = -cameraPos.z() / direction.z();
		out = cameraPos + t * direction;
		
		return true;
	}

	bool Player::IntersectXZPlane(const Eigen::Vector3f& cameraPos, const Eigen::Vector3f direction, Eigen::Vector3f& out)
	{
		using namespace Eigen;

		if (std::fabs(direction.y()) < 1e-6) 
		{
			return false;
		}

		float t = -cameraPos.y() / direction.y();
		out = cameraPos + t * direction;

		return true;
	}

	void Player::ResetCamera()
	{
		ResetCameraArm();
		ResetCameraCore();
	}

	void Player::ResetCameraArm()
	{
		data.cameraArm->SetLocalPosition(Eigen::Vector3f::Zero());
		data.cameraArm->SetLocalRotation(data.modelCore->GetLocalRotation());
	}

	void Player::ResetCameraCore()
	{
		data.cameraCore->SetLocalPosition(Eigen::Vector3f(0.f, 0.f, -9.f));
		data.cameraCore->SetLocalRotation(Eigen::Quaternionf::Identity());
	}

	void Player::RotateCameraArm()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();
		const float deltaTime = time.GetDeltaTime();

		const float yawAngle = currInput.Rstick.X * data.sensitivity * deltaTime * currInput.Rstick.Size;
		RotateCameraArmYaw(yawAngle);

		const float pitchAngle = -currInput.Rstick.Y * data.sensitivity * deltaTime * currInput.Rstick.Size;
		RotateCameraArmPitch(pitchAngle);
	}

	void Player::RotateCameraArmYaw(float yawAngle)
	{
		data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
	}

	void Player::RotateCameraArmPitch(float pitchAngle)
	{
		float deltaAngle = 0.f;
		data.xAngle += pitchAngle;

		if (data.xAngle > data.limitHighAngle)
		{
			deltaAngle = 0.f;
			data.xAngle = data.limitHighAngle;
		}
		else if (data.xAngle < data.limitLowAngle)
		{
			deltaAngle = 0.f;
			data.xAngle = data.limitLowAngle;
		}
		else
		{
			deltaAngle = pitchAngle;
		}

		const Eigen::Vector3f cameraRight = data.cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();
		data.cameraArm->Rotate(cameraRight, deltaAngle);
	}

	void Player::CameraLookTo(const Eigen::Vector3f& _direction)
	{
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), _direction);

		data.cameraArm->SetWorldRotation(targetRotation);
	}

	void Player::CameraLookAt(const Eigen::Vector3f& _position)
	{
		using namespace Eigen;

		Vector3f pos = data.cameraCore->GetWorldPosition();
		Vector3f direction = _position - pos;
		direction.normalize();

		Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
		Eigen::Vector3f right = up.cross(direction).normalized();

		Eigen::Vector3f newUp = direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = direction;

		data.cameraCore->SetWorldRotation(Eigen::Quaternionf(rotation));
	}
#pragma endregion camera

	bool Player::RaycastFromCamera(
		float _distance, 
		PurahEngine::RigidBody** _outBody,
		PzObject** _outPzObject,
		Rewindable** _outRewindable
	)
	{
		const Eigen::Vector3f from = data.cameraCore->GetWorldPosition();
		Eigen::Matrix3f rotate = data.cameraCore->GetWorldRotation().toRotationMatrix();
		Eigen::Vector3f to = rotate * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		ZonaiPhysics::ZnQueryInfo info;

		unsigned int layers = data.attachRaycastLayers;

		const bool block = PurahEngine::Physics::Raycast(from, to, _distance, layers, info);

		if (block)
		{
			const PurahEngine::Collider* shape = static_cast<PurahEngine::Collider*>(info.colliderData);

			if (!shape)
				return false;

			const PurahEngine::GameObject* obj = shape->GetGameObject();
			PurahEngine::RigidBody* body = obj->GetComponent<PurahEngine::RigidBody>();

			if (!body)
				return false;

			if (_outBody)
				*_outBody = body;
			if (_outPzObject)
				*_outPzObject = obj->GetComponent<PzObject>();
			if (_outRewindable)
				*_outRewindable = obj->GetComponent<Rewindable>();

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				shape->GetGameObject()->GetName(), 800, 600, 100, 100, 30, 255, 255, 255, 255);
		}

		return block;
	}


	void Player::ChangeAbilityState(AbilityState _state)
	{
		currState = _state;
	}

	void Player::ChangePlayerState(PlayerState _state)
	{
		currPlayerState = _state;
	}

#pragma region 직렬화
	void Player::PreSerialize(json& jsonData) const
	{}

	void Player::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		auto moveSpeed = data.moveSpeed;
		PREDESERIALIZE_VALUE(moveSpeed);
		data.moveSpeed = moveSpeed;

		auto sensitivity = data.sensitivity;
		PREDESERIALIZE_VALUE(sensitivity);
		data.sensitivity = sensitivity;

		auto jumpPower = data.jumpPower;
		PREDESERIALIZE_VALUE(jumpPower);
		data.jumpPower = jumpPower;

		int cameraCollisionLayers = data.cameraCollisionLayers;
		PREDESERIALIZE_VALUE(cameraCollisionLayers);
		data.cameraCollisionLayers = cameraCollisionLayers;

		float cameraCollisionRadius = data.cameraCollisionRadius;
		PREDESERIALIZE_VALUE(cameraCollisionRadius);
		data.cameraCollisionRadius = cameraCollisionRadius;

		auto attachRaycastLayers = data.attachRaycastLayers;
		PREDESERIALIZE_VALUE(attachRaycastLayers);
		data.attachRaycastLayers = attachRaycastLayers;

		float attachRaycastDistance = data.attachRaycastDistance;
		PREDESERIALIZE_VALUE(attachRaycastDistance);
		data.attachRaycastDistance = attachRaycastDistance;

		auto idleAnimation = data.idleAnimation;
		PREDESERIALIZE_WSTRING(idleAnimation);
		data.idleAnimation = idleAnimation;

		auto runningAnimation = data.runningAnimation;
		PREDESERIALIZE_WSTRING(runningAnimation);
		data.runningAnimation = runningAnimation;
	}

	void Player::PostSerialize(json& jsonData) const
	{}

	void Player::PostDeserialize(const json& jsonData)
	{
		auto playerRigidbody = data.playerRigidbody;
		POSTDESERIALIZE_PTR(playerRigidbody);
		data.playerRigidbody = playerRigidbody;

		auto modelCore = data.modelCore;
		POSTDESERIALIZE_PTR(modelCore);
		data.modelCore = modelCore;

		auto cameraArm = data.cameraArm;
		POSTDESERIALIZE_PTR(cameraArm);
		data.cameraArm = cameraArm;

		auto cameraCore = data.cameraCore;
		POSTDESERIALIZE_PTR(cameraCore);
		data.cameraCore = cameraCore;

		auto animator = data.animator;
		POSTDESERIALIZE_PTR(animator);
		data.animator = animator;

		auto crossHead = data.crossHead;
		POSTDESERIALIZE_PTR(crossHead);
		data.crossHead = crossHead;

		auto cameraSweep = data.cameraSweep;
		POSTDESERIALIZE_PTR(cameraSweep);
		data.cameraSweep = cameraSweep;
	}

#pragma endregion 직렬화
}
