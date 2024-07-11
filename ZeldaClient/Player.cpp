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
#include "AttachSystem.h"

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

#pragma region Initialize
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

		data.coreTargetRotation = Eigen::Quaternionf::Identity();

		data.coreSelectPosition = data.coreDefaultPosition + Eigen::Vector3f{0.5f, 0.5f, 0.f};

		highPosition = Eigen::Vector3f(0.f, 0.f, -10.f);
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
		{
			// 애니메이션 문자열 맵 초기화
			animationString = {
				{IDLE, animData.idleAnimation},
				{WALK, animData.walkingAnimation},
				{RUN, animData.runningAnimation},
				{JUMP, animData.jumpAnimation},
				{JUMPING, animData.jumpingAnimation},
				{LANDING, animData.landingAnimation},
				{ABILITY_IDLE, animData.holdIdleAnimation},
				{ABILITY_FRONT, animData.holdFrontAnimation},
				{ABILITY_BACK, animData.holdBackAnimation},
				{ABILITY_RIGHT, animData.holdRightAnimation},
				{ABILITY_LEFT, animData.holdLeftAnimation}
			};
		}

		{
			// 애니메이션 상태 및 속도 컨트롤러 맵 초기화
			for (const auto& [type, animation] : animationString) 
			{
				AddAnimationState(animationState, type, animation, data.animator);
				
				if (type == WALK || type == RUN || type >= ABILITY_FRONT) 
				{
					AddAnimationSpeedController(animationSpeedController, type, animation, data.animator);
				}
			}
		}
	}

	// 공통된 애니메이션 상태 추가 함수
	void Player::AddAnimationState(
		std::map<PlayerState, std::function<void()>>& stateMap, 
		PlayerState type, const std::wstring& animation, 
		PurahEngine::Animator* animator)
	{
		stateMap[type] = [animation, animator]() 
			{
				animator->Play(animation);
			};
	}

	// 공통된 애니메이션 속도 컨트롤러 추가 함수
	void Player::AddAnimationSpeedController(
		std::map<PlayerState, std::function<void(float)>>& speedMap, 
		PlayerState type, const std::wstring& animation, 
		PurahEngine::Animator* animator)
	{
		speedMap[type] = [animation, animator](float speed) 
			{
				animator->SetPlaySpeed(animation, speed);
			};
	}
#pragma endregion Initialize

#pragma region Debug
	void Player::DebugDraw()
	{
		using namespace Eigen;

		DrawStateInfo();

		Vector3f currvelo = data.playerRigidbody->GetLinearVelocity();
		float spd = currvelo.norm();
		
		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"Player Current Speed : " + std::to_wstring(spd),
			400, 100, 
			100, 100, 
			15, 
			255, 255, 255, 255
		);

		DrawJumpInfo();
	}

	void Player::DrawStateInfo() const
	{
		std::wstring str = GetStateString(data.state);
		std::wstring str0 = GetStateString(currState);

		PurahEngine::GraphicsManager::GetInstance().DrawString(L"SELETE STATE : " + str, 100, 100, 300, 100, 15, 255, 255, 255, 255);
		PurahEngine::GraphicsManager::GetInstance().DrawString(L"PLAYER STATE : " + str0, 100, 200, 400, 100, 15, 255, 255, 255, 255);
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
		std::wstring jumpStatus = data.isGrounded ? L"Jumping" : L"can Jump";
		PurahEngine::GraphicsManager::GetInstance().DrawString(jumpStatus, 500, 200, 400, 200, 15, 255, 255, 255, 255);
	}
#pragma endregion Debug

#pragma region Event
	void Player::Start()
	{
		InitializeGamePad();
		InitializeDefaultPositions();
		InitializeAbilitySystem();
		InitializeStateSystem();
		InitializeLerpFunctions();
		AttachSystem::Instance()->SetOutlineColor(&color0, &color1, &color2);
	}

	void Player::Update()
	{
		using namespace Eigen;

		prevState = currState;
		prevPlayerState = currPlayerState;
		data.isGrounded = GroundCheck();

		if (!data.isGrounded)
		{
			stateSystem[currState]->StateCancel();
		}

		if (!data.stopUpdate)
		{
			HandleInput();
			UpdateAbilityState();

			animData.animationSpeed = currInput.Lstick.Size;
		}
		else
		{
			animData.animationSpeed = 0.f;
		}

		Vector3f pos = GetGameObject()->GetTransform()->GetWorldPosition();
	}

	void Player::LateUpdate()
	{
		UpdatePlayerAnimationState();
		PostUpdateAbilityState();

		UpdateCameraLerp();
		CharacterDisable();

		if (data.debugMode)
		{
			DebugDraw();
		}
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
	}

	void Player::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
	}
#pragma endregion Event

	void Player::SetStopUpdate(bool _value)
	{
		stateSystem[currState]->StateCancel();

		data.stopUpdate = _value;
	}

#pragma region Update
	void Player::UpdateAbilityState()
	{
		if (prevState != currState)
		{
			if (stateSystem.contains(prevState))
				stateSystem[prevState]->StateExit();

			if (stateSystem.contains(currState))
				stateSystem[currState]->StateEnter();
		}
		else
		{
			if (stateSystem.contains(currState))
				stateSystem[currState]->StateStay();
		}
	}

	void Player::PostUpdateAbilityState()
	{
		if (prevState != currState)
			return;

		if (stateSystem.contains(currState))
		{
			stateSystem[currState]->PostStateStay();
		}
	}


	void Player::UpdatePlayerAnimationState()
	{
		if (prevPlayerState != currPlayerState)
		{
			if (animationState.contains(currPlayerState))
				animationState[currPlayerState]();
		}
		else
		{
			if (animationSpeedController.contains(currPlayerState))
				animationSpeedController[currPlayerState](animData.animationSpeed);
		}
	}
#pragma endregion Update

	void Player::ChangeAbilityState(AbilityState _state)
	{
		currState = _state;
	}

	void Player::ChangePlayerAnimationState(PlayerState _state)
	{
		currPlayerState = _state;
	}

#pragma region Input
	void Player::HandleInput()
	{
		if (gamePad->IsConnected())
		{
			HandleGamePadInput();
		}
		else
		{
			HandleKeyboardInput();
		}

		HandleDebugToggle();
	}

	void Player::HandleDebugToggle()
	{
		bool keyCtrl = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_CONTROL);
		bool keyD = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_D);
		bool keyShift = PurahEngine::InputManager::Getinstance().IsKeyReleased(PurahEngine::eKey::eKEY_SHIFT);

		if (keyCtrl && keyD && keyShift)
		{
			data.debugMode = !data.debugMode;
		}
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

	void Player::HandleKeyboardInput()
	{
		HandleMovementInput();
		stateSystem[currState]->Stick_L();
		stateSystem[currState]->Stick_R();

		HandleCameraRotationInput();
		HandleActionInput();
		HandleAbilityInput();

	}

	void Player::HandleMovementInput()
	{
		bool moveForward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_W);
		bool moveBackward = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_S);
		bool moveLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_A);
		bool moveRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_D);

		currInput.Lstick.Y = moveForward ? 1.0f : (moveBackward ? -1.0f : 0.0f);
		currInput.Lstick.X = moveLeft ? -1.0f : (moveRight ? 1.0f : 0.0f);

		float magnitude = std::sqrt(currInput.Lstick.X * currInput.Lstick.X + currInput.Lstick.Y * currInput.Lstick.Y);
		currInput.Lstick.Size = std::clamp(magnitude, 0.0f, 1.0f);
	}

	void Player::HandleCameraRotationInput()
	{
		bool rotateUp = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_UP);
		bool rotateDown = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_DOWN);
		bool rotateLeft = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_LEFT);
		bool rotateRight = PurahEngine::InputManager::Getinstance().IsKeyPressed(PurahEngine::eKey::eKEY_RIGHT);

		currInput.Rstick.Y = rotateUp ? 1.0f : (rotateDown ? -1.0f : 0.0f);
		currInput.Rstick.X = rotateLeft ? -1.0f : (rotateRight ? 1.0f : 0.0f);

		float magnitude = std::sqrt(currInput.Rstick.X * currInput.Rstick.X + currInput.Rstick.Y * currInput.Rstick.Y);
		currInput.Rstick.Size = std::clamp(magnitude, 0.0f, 1.0f);
	}

	void Player::HandleActionInput()
	{
		bool jump = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_SPACE);
		if (jump)
			stateSystem[currState]->Click_A();

		bool select = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_F);
		if (select)
			stateSystem[currState]->Click_B();

		bool attach = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Z);
		if (attach)
			stateSystem[currState]->Click_X();
	}

	void Player::HandleAbilityInput()
	{
		bool abillity = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_Q);
		if (abillity)
			stateSystem[currState]->Click_LB();

		bool rotate = PurahEngine::InputManager::Getinstance().IsKeyDown(PurahEngine::eKey::eKEY_E);
		if (rotate)
			stateSystem[currState]->Click_RB();
	}
#pragma endregion Input

#pragma region Player
	bool Player::SlopeCheck(const Eigen::Vector3f& normal) const
	{
		// up 벡터랑 노말과 각도가 혀용 범위 내인가?
		const Eigen::Vector3f up{ 0.f, 1.f, 0.f };
		float cosTheta = up.dot(normal);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float slope = std::acosf(cosTheta);

		return slope <= data.slopeLimit;
	}

	bool Player::GroundCheck()
	{
		using namespace Eigen;

		Vector3f start = data.playerRigidbody->GetPosition() - Vector3f::UnitY() * 0.025f;
		Vector3f to = -Vector3f::UnitY();
		float dis = 0.1f;
		int layers = data.cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Raycast(start, to, dis, layers, info);

		if (hit)
		{
			data.lastGroundNormal = info.normal;
			return SlopeCheck(info.normal);
		}

		return hit;
	}

	bool Player::SweepTest(const Eigen::Vector3f& _movement, float minDist, float stepOffset, int collisionLayers)
	{
		using namespace Eigen;

		// 플레이어 발 위치
		Vector3f footPosition = GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3f startPosition = footPosition + Vector3f::UnitY();	// 스윕 시작
		Vector3f direction = _movement.normalized();			// 스윕 방향
		float distance = _movement.norm();						// 스윕 거리

		const float radius = 0.5f;				// 스윕 모양
		const float height = 1.f;

		ZonaiPhysics::ZnQueryInfo info;			// 반환 값

		bool hit = PurahEngine::Physics::Capsulecast(
			radius, height, 
			startPosition, Quaternionf::Identity(),
			direction, distance,
			collisionLayers, info);

		if (hit)
		{
			// 스윕이 충돌한 위치와 플레이어 위치랑 비교함
			info.position;

			// 
		}

		return true;
	}

	bool Player::TryJump()
	{
		using namespace Eigen;

		if (data.isGrounded)
		{
			Eigen::Vector3f power = Eigen::Vector3f::UnitY() * data.jumpPower;
			data.playerRigidbody->AddForce(power, ZonaiPhysics::ForceType::Accelration);
		}

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
			if (SlopeCheck(normal))
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

	bool Player::CanMove(const Eigen::Vector3f& direction, float distance, float stepOffset)
	{
		using namespace Eigen;

		// 이동 경로에 장애물이 있는지 확인하기 위해 SweepTest를 수행합니다.
		Vector3f movement = direction * distance;
		return !SweepTest(movement, 0.01f, stepOffset, data.cameraCollisionLayers);
	}

	bool Player::TryPlayerMove(float _moveSpeed)
	{
		using namespace Eigen;

		// 카메라의 전방 벡터를 계산
		const Vector3f cameraFront = data.cameraArm->GetFront();
		const Vector3f forward = Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Vector3f right = Vector3f::UnitY().cross(forward);

		// 이동 방향 벡터를 계산
		const Vector3f movementDirection = forward * currInput.Lstick.Y + right * currInput.Lstick.X;
		const float moveSpeed = _moveSpeed * currInput.Lstick.Size;

		// 속도 벡터를 계산
		Vector3f currentVelocity = data.playerRigidbody->GetLinearVelocity();
		Vector3f targetVelocity = moveSpeed * movementDirection;
		Vector3f additionalVelocity = targetVelocity - currentVelocity;
		additionalVelocity.y() = 0.f; // 수직 방향 속도는 0으로 설정

		//// 속도를 제한?
		// Vector3f direction = additionalVelocity.normalized();
		// float magnitude = additionalVelocity.norm();
		// magnitude = std::clamp(magnitude, 0.f, moveSpeed);
		// additionalVelocity = direction * magnitude;

		// 속도를 적용시킴
		data.playerRigidbody->AddForce(additionalVelocity, ZonaiPhysics::Velocity_Change);

		Vector3f gravity;
		if (currentVelocity.y() > 0.f)
			gravity = PurahEngine::Physics::GetGravity();
		else if (currentVelocity.y() < 0.f)
			gravity = PurahEngine::Physics::GetGravity() * 1.5f;
		else
			gravity = Vector3f::Zero();

		data.playerRigidbody->AddForce(gravity, ZonaiPhysics::Velocity_Change);

		// 경사면에서 미끄러짐 처리
		//if (data.isGrounded) 
		//{
		//	additionalVelocity.x() += (1.f - additionalVelocity.y()) * data.lastGroundNormal.x() * (1.f - data.slideFriction);
		//	additionalVelocity.z() +w= (1.f - additionalVelocity.y()) * data.lastGroundNormal.z() * (1.f - data.slideFriction);
		//	data.playerRigidbody->AddForce(additionalVelocity, ZonaiPhysics::Velocity_Change);
		//}

		return currInput.Lstick.Size >= 1e-6;
	}
#pragma endregion Player

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

#pragma region Camera
	void Player::UpdateDefaultCamera()
	{
		RotateCameraArm();
		UpdateDefaultCameraCore();
	}

	void Player::UpdateSelectCamera()
	{
		RotateCameraArm();
		UpdateSelectCameraCore();
	}

	// default 상태에서 camera core의 위치를 계산하는 함수 
	void Player::UpdateDefaultCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, false);
		
		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void Player::UpdateSelectCameraCore()
	{
		using namespace Eigen;

		Vector3f localPosition = Vector3f::Zero();
		Vector3f worldPosition = Vector3f::Zero();
		// 각도에 따라 위치를 우선 제한함
		CalculateDefaultCameraCorePosition(localPosition, worldPosition, true);

		// 오브젝트에 충돌하면 처리함
		if (ResolveCameraCollision(localPosition, worldPosition))
		{
			SetCameraCoreWorldTargetPosition(worldPosition);
		}
		else
		{
			SetCameraCoreLocalTargetPosition(localPosition);
		}
	}

	void Player::CalculateDefaultCameraCorePosition(Eigen::Vector3f& localOut, Eigen::Vector3f& worldOut, bool _isSelect)
	{
		using namespace Eigen;

		const float ease = data.xAngle >= 0.f ? data.xAngle / data.limitHighAngle : data.xAngle / data.limitLowAngle;
		auto easingFunc = data.xAngle >= 0.f ?
			[](float x) { return 1.0f - (1.0f - x) * (1.0f - x); } :
			[](float x) { return 1.0f - std::pow(1.0f - x, 5.0f); };

		const Vector3f& diff = data.xAngle >= 0.f ? differenceHigh : differenceLow;

		Vector3f offset = _isSelect ? Vector3f{ 0.5f, 0.5f, 0.f } : Vector3f::Zero();
		Vector3f pos = data.coreDefaultPosition + offset;
		// Vector3f pos = data.coreTargetPosition;
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
		static bool prevHit = false;

		float radius = data.cameraCollisionRadius;
		const Vector3f coreLocalPos = localIn;
		const Vector3f coreWorldPos = worldIn;

		// const Vector3f coreLocalDir = data.cameraCore->GetLocalRotation() * Vector3f::UnitZ();
		// const Vector3f localStart = CalculateCameraFocusPosition(coreLocalPos, coreLocalDir);

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		// world.translate(localStart);
		const Vector3f worldStart = world.translation();

		Vector3f worldDir = coreWorldPos - worldStart;
		const float dis = worldDir.norm();
		worldDir.normalize();

		if (worldDir.isZero())
		{
			worldDir = data.cameraCore->GetWorldRotation() * -Vector3f::UnitZ();
		}

		unsigned int layers = data.cameraCollisionLayers;
		ZonaiPhysics::ZnQueryInfo info;

		bool hit = PurahEngine::Physics::Spherecast(
			radius, 
			worldStart, Quaternionf::Identity(), 
			worldDir, dis, 
			layers, 
			info
		);

		//if (!hit)
		//{
		//	return hit;
		//}

		if (hit)
		{
			Vector3f position = worldStart + worldDir * info.distance;
			Affine3f worldT{ data.cameraArm->GetWorldMatrix() };
			Vector3f localPos = worldT.inverse() * position;

			if ((info.distance < prevDistance) && (!prevHit && hit))
			{
				data.cameraCore->SetLocalPosition(localPos);
			}

			data.coreTargetPosition = localPos;

			localIn = localPos;
			worldIn = position;
		}
	
		prevDistance = hit ? info.distance : FLT_MAX;
		prevHit = hit;

		return hit;
	}

	void Player::UpdateCameraPositionLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Vector3f currPos = data.cameraCore->GetLocalPosition();
		Vector3f target = data.coreTargetPosition;
		Vector3f diff = target - currPos;
		float dis = diff.norm();

		Vector3f pos = Vector3f::Zero();
		if (dis > 1e-6)
		{
			float inv = dt / data.cameraLerpTime;
			// float inv = 0.001f / data.cameraLerpTime;
			inv = std::clamp(inv, 0.f, 1.f);
			pos = lerp(currPos, target, inv);
		}
		else
		{
			pos = target;
		}

		data.cameraCore->SetLocalPosition(pos);
	}

	void Player::UpdateCameraRotationLerp()
	{
		using namespace Eigen;

		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		Quaternionf currRot = data.cameraCore->GetLocalRotation();
		Quaternionf currRotM = Quaternionf(-currRot.w(), -currRot.x(), -currRot.y(), -currRot.z());
		Quaternionf curr = currRot;
		Quaternionf target = data.coreTargetRotation;

		float costheta0 = currRot.dot(target);
		float costheta1 = currRotM.dot(target);

		if (costheta0 < costheta1)
		{
			curr = currRotM;
		}

		float inv = dt / data.cameraLerpTime;
		// float inv = 0.001f / data.cameraLerpTime;
		inv = std::clamp(inv, 0.f, 1.f);
		Quaternionf newRot = curr.slerp(inv, target);

		data.cameraCore->SetLocalRotation(newRot);
	}

	void Player::UpdateCameraLerp()
	{
		UpdateCameraPositionLerp();
		UpdateCameraRotationLerp();
	}

	void Player::CharacterDisable()
	{
		using namespace Eigen;
		
		PurahEngine::Camera* mainCamera = PurahEngine::SceneManager::GetInstance().GetMainCamera();
		Vector3f cameraPos = mainCamera->GetGameObject()->GetTransform()->GetWorldPosition();
		Vector3f cameraArmPos = data.cameraArm->GetWorldPosition();
		Vector3f vec = cameraArmPos - cameraPos;
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
		data.coreTargetPosition = _localPosision;
	}

	void Player::SetCameraCoreLocalTargetRotation(const Eigen::Quaternionf& _localRotation)
	{
		data.coreTargetRotation = _localRotation;
	}

	void Player::SetCameraCoreWorldTargetPosition(const Eigen::Vector3f& _worldPosision)
	{
		using namespace Eigen;

		Affine3f world{ data.cameraArm->GetWorldMatrix() };
		Vector3f localPos = world.inverse() * _worldPosision;
		
		data.coreTargetPosition = localPos;
	}

	void Player::SetCameraCoreWorldTargetRotation(const Eigen::Quaternionf& _worldRotation)
	{
		using namespace Eigen;

		Quaternion world{ data.cameraArm->GetWorldRotation() };
		Quaternionf localRot = world.inverse() * _worldRotation;

		data.coreTargetRotation = localRot;
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

	void Player::ResetCameraArmAndCameraCore()
	{
		using namespace Eigen;

		Vector3f corePos = data.cameraCore->GetWorldPosition();
		Quaternionf coreRot = data.cameraCore->GetWorldRotation();

		data.xAngle = 0.f;
		data.cameraArm->SetLocalPosition(data.armDefaultPosition);
		data.cameraArm->SetLocalRotation(data.modelCore->GetLocalRotation());

		data.cameraCore->SetWorldRotation(coreRot);
		data.cameraCore->SetWorldPosition(corePos);
	}

	void Player::ResetCameraCoreTarget()
	{
		SetCameraCoreLocalTargetPosition(data.coreDefaultPosition);
		SetCameraCoreLocalTargetRotation(data.coreDefaultRotation);
	}

	void Player::ResetCamera()
	{
		ResetCameraArm();
		ResetCameraCore();
	}

	void Player::ResetCameraArm()
	{
		data.xAngle = 0.f;
		data.cameraArm->SetLocalPosition(data.armDefaultPosition);
		data.cameraArm->SetLocalRotation(data.modelCore->GetLocalRotation());
	}

	void Player::ResetCameraCore()
	{
		data.cameraCore->SetLocalPosition(data.coreDefaultPosition);
		data.cameraCore->SetLocalRotation(data.coreDefaultRotation);
	}

	void Player::SetCameraArmFoward(const Eigen::Vector3f& _direction)
	{
		using namespace Eigen;

		Eigen::Vector3f up{ Vector3f::UnitY() };
		Eigen::Vector3f right = up.cross(_direction).normalized();

		Eigen::Vector3f newUp = _direction.cross(right).normalized();

		Eigen::Matrix3f rotation;
		rotation.col(0) = right;
		rotation.col(1) = newUp;
		rotation.col(2) = _direction;

		data.cameraArm->SetWorldRotation(Eigen::Quaternionf(rotation));

		float cosTheta = _direction.dot(up);
		cosTheta = std::clamp(cosTheta, -1.f, 1.f);
		float theta = std::acosf(cosTheta);
		data.xAngle = theta * (180.f / std::numbers::pi_v<float>);
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
		float deltaAngle = pitchAngle;
		data.xAngle += pitchAngle;

		if (data.xAngle > data.limitHighAngle)
		{
			deltaAngle -= (data.xAngle - data.limitHighAngle);
			data.xAngle = data.limitHighAngle;
		}
		else if (data.xAngle < data.limitLowAngle)
		{
			deltaAngle -= (data.xAngle - data.limitLowAngle);
			data.xAngle = data.limitLowAngle;
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
#pragma endregion Camera

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

#pragma region 직렬화
	void Player::PreSerialize(json& jsonData) const
	{}

	void Player::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();

		{
			auto moveSpeed = data.moveSpeed;
			PREDESERIALIZE_VALUE(moveSpeed);
			data.moveSpeed = moveSpeed;

			auto sensitivity = data.sensitivity;
			PREDESERIALIZE_VALUE(sensitivity);
			data.sensitivity = sensitivity;

			auto jumpPower = data.jumpPower;
			PREDESERIALIZE_VALUE(jumpPower);
			data.jumpPower = jumpPower;
		}

		{
			Eigen::Vector4f outlineColor0;
			PREDESERIALIZE_VECTOR4F(outlineColor0);
			color0 = outlineColor0;

			Eigen::Vector4f outlineColor1;
			PREDESERIALIZE_VECTOR4F(outlineColor1);
			color1 = outlineColor1;
			
			Eigen::Vector4f outlineColor2;
			PREDESERIALIZE_VECTOR4F(outlineColor2);
			color2 = outlineColor2;
		}

		{
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
		}

		{
			auto idleAnimation = animData.idleAnimation;
			PREDESERIALIZE_WSTRING(idleAnimation);
			animData.idleAnimation = idleAnimation;

			auto walkingAnimation = animData.walkingAnimation;
			PREDESERIALIZE_WSTRING(walkingAnimation);
			animData.walkingAnimation = walkingAnimation;

			auto runningAnimation = animData.runningAnimation;
			PREDESERIALIZE_WSTRING(runningAnimation);
			animData.runningAnimation = runningAnimation;

			auto jumpAnimation = animData.jumpAnimation;
			PREDESERIALIZE_WSTRING(jumpAnimation);
			animData.jumpAnimation = jumpAnimation;

			auto jumpingAnimation = animData.jumpingAnimation;
			PREDESERIALIZE_WSTRING(jumpingAnimation);
			animData.jumpingAnimation = jumpingAnimation;

			auto landingAnimation = animData.landingAnimation;
			PREDESERIALIZE_WSTRING(landingAnimation);
			animData.landingAnimation = landingAnimation;

			auto holdIdleAnimation = animData.holdIdleAnimation;
			PREDESERIALIZE_WSTRING(holdIdleAnimation);
			animData.holdIdleAnimation = holdIdleAnimation;

			auto holdFrontAnimation = animData.holdFrontAnimation;
			PREDESERIALIZE_WSTRING(holdFrontAnimation);
			animData.holdFrontAnimation = holdFrontAnimation;

			auto holdBackAnimation = animData.holdBackAnimation;
			PREDESERIALIZE_WSTRING(holdBackAnimation);
			animData.holdBackAnimation = holdBackAnimation;

			auto holdRightAnimation = animData.holdRightAnimation;
			PREDESERIALIZE_WSTRING(holdRightAnimation);
			animData.holdRightAnimation = holdRightAnimation;

			auto holdLeftAnimation = animData.holdLeftAnimation;
			PREDESERIALIZE_WSTRING(holdLeftAnimation);
			animData.holdLeftAnimation = holdLeftAnimation;
		}
	}

	void Player::PostSerialize(json& jsonData) const
	{}

	void Player::PostDeserialize(const json& jsonData)
	{
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
		}

		auto groundChechRaycast = data.groundChechRaycast;
		POSTDESERIALIZE_PTR(groundChechRaycast);
		data.groundChechRaycast = groundChechRaycast;
		
		{
			auto attachLowCamera0 = data.attachLowCamera0;
			POSTDESERIALIZE_PTR(attachLowCamera0);
			data.attachLowCamera0 = attachLowCamera0;

			auto attachLowCamera1 = data.attachLowCamera1;
			POSTDESERIALIZE_PTR(attachLowCamera1);
			data.attachLowCamera1 = attachLowCamera1;

			auto attachDefaultCamera0 = data.attachDefaultCamera0;
			POSTDESERIALIZE_PTR(attachDefaultCamera0);
			data.attachDefaultCamera0 = attachDefaultCamera0;

			auto attachDefaultCamera1 = data.attachDefaultCamera1;
			POSTDESERIALIZE_PTR(attachDefaultCamera1);
			data.attachDefaultCamera1 = attachDefaultCamera1;

			auto attachHighCamera0 = data.attachHighCamera0;
			POSTDESERIALIZE_PTR(attachHighCamera0);
			data.attachHighCamera0 = attachHighCamera0;

			auto attachHighCamera1 = data.attachHighCamera1;
			POSTDESERIALIZE_PTR(attachHighCamera1);
			data.attachHighCamera1 = attachHighCamera1;
		}
	}

#pragma endregion 직렬화
}
