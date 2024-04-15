#include <algorithm>
#include <iostream>

#include "IState.h"
#include "DefaultState.h"
#include "HoldState.h"
#include "AttatchState.h"
#include "RewindState.h"

#include "Player.h"

#include "../GraphicsTest/CoreSystem.h"

namespace Phyzzle
{
	Player::~Player()
	{
		
	}

	void Player::Start()
	{
		PurahEngine::GamePadManager::AddGamePad(0);

		gamePad = PurahEngine::GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		data.armDefaultPosition = data.cameraArm->GetLocalPosition();
		data.armDefaultRotation = data.cameraArm->GetLocalRotation();
		data.armStartPosition = data.armDefaultPosition;
		data.armStartRotation = data.armDefaultRotation;
		data.armTargetPosition = data.armDefaultPosition;
		data.armTargetRotation = data.armDefaultRotation;

		data.coreDefaultPosition = data.cameraCore->GetLocalPosition();
		data.coreDefaultRotation = data.cameraCore->GetLocalRotation();
		data.coreStartPosition = data.coreDefaultPosition;
		data.coreStartRotation = data.coreDefaultRotation;
		data.coreTargetPosition = data.coreDefaultPosition;
		data.coreTargetRotation = data.coreDefaultRotation;

		highPosition = Eigen::Vector3f(0.f, 0.f, -20.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - data.coreDefaultPosition;
		differenceLow = lowPosition - data.coreDefaultPosition;

		lerp = 
			[this](const Eigen::Vector3f start, const Eigen::Vector3f end, float _t) -> Eigen::Vector3f
			{
				if (start == end)
					return start;

				return start + _t * (end - start);
			};

		slerp =
			[this](const Eigen::Quaternionf start, const Eigen::Quaternionf end, float _t) -> Eigen::Quaternionf
			{
				if (start == end)
					return start;

				return 	start.slerp(_t, end);
			};

		stateSystem.insert(std::make_pair(State::DEFAULT, new DefaultState(this)));
		stateSystem.insert(std::make_pair(State::HOLD, new HoldState(this)));
		stateSystem.insert(std::make_pair(State::ATTATCH, new AttatchState(this)));
		stateSystem.insert(std::make_pair(State::REWIND, new RewindState(this)));
	}

	void Player::Update()
	{
		prevState = currState;

		DebugDraw();
		GamePadInput();

		stateSystem[currState]->Input();

		if (prevState != currState)
		{
			stateSystem[prevState]->StateExit();
			stateSystem[currState]->StateEnter();
		}

		(*stateSystem[currState])();

		// CameraUpdate();
	}

	void Player::OnCollisionEnter(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		JumpCheck(zn_collision, collider);
	}

	void Player::OnCollisionStay(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		if (!data.jumping)
			return;

		JumpCheck(zn_collision, collider);
	}

	void Player::DebugDraw()
	{
		std::wstring str{};
		{
			if (data.state == HOLD)
			{
				str = L"HOLD";
			}
			else if (data.state == ATTATCH)
			{
				str = L"ATTATCH";
			}
			else if (data.state == REWIND)
			{
				str = L"REWIND";
			}
			else if (data.state == LOCK)
			{
				str = L"LOCK";
			}
			else
			{
				str = L"ERROR";
			}
		}

		std::wstring str0{};
		{
			if (currState == HOLD)
			{
				str0 = L"HOLD";
			}
			else if (currState == ATTATCH)
			{
				str0 = L"ATTATCH";
			}
			else if (currState == REWIND)
			{
				str0 = L"REWIND";
			}
			else if (currState == LOCK)
			{
				str0 = L"LOCK";
			}
			else
			{
				str0 = L"DEFAULT";
			}
		}

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"SELETE STATE : " + str, 
			100, 100, 
			300, 100, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"PLAYER STATE : " + str0,
			100, 200, 
			400, 100, 15,
			255, 255, 255, 255);

		PurahEngine::GraphicsManager::GetInstance().DrawString(
			L"X",
			945, 510,
			200, 100, 50,
			255, 255, 255, 255);
	}

	void Player::GamePadInput()
	{
		if (gamePad->IsConnected())
		{
			currInput.Lstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, currInput.Lstick.X, currInput.Lstick.Y);
			currInput.Rstick.Size = gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, currInput.Rstick.X, currInput.Rstick.Y);

			stateSystem[currState]->Stick_L();
			stateSystem[currState]->Stick_R();

			currInput.LTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			currInput.RTrigger = gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

			stateSystem[currState]->Trigger_L();
			stateSystem[currState]->Trigger_R();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_SHOULDER_L))
				stateSystem[currState]->Click_LB();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_SHOULDER_R))
				stateSystem[currState]->Click_RB();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_A))
				stateSystem[currState]->Click_A();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_B))
				stateSystem[currState]->Click_B();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_X))
				stateSystem[currState]->Click_X();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_Y))
				stateSystem[currState]->Click_Y();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_UP))
				stateSystem[currState]->Click_DUp();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_DOWN))
				stateSystem[currState]->Click_DDown();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_LEFT))
				stateSystem[currState]->Click_DLeft();
			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_RIGHT))
				stateSystem[currState]->Click_DRight();
		}
	}

	void Player::Jump()
	{
		if (!data.jumping)
		{
			auto curr = data.playerRigidbody->GetLinearVelocity();
			curr.y() = data.jumpPower;
			data.playerRigidbody->SetLinearVelocity(curr);
			data.jumping = true;
		}
	}

	void Player::JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		const auto velo = zn_collision.thisPostLinearVelocity;

		const Eigen::Vector3f up{ 0.f, 1.f, 0.f };
		// const Eigen::Vector3f direction{ zn_collision.impulses.normalized() };
		const Eigen::Vector3f direction{ velo };
		float cosTheta1 = up.dot(direction);
		cosTheta1 = std::clamp(cosTheta1, -1.f, 1.f);

		if ((cosTheta1 >= 0.5f))
		{
			data.jumping = false;
		}
	}

	void Player::Move(float _moveSpeed, bool _cameraLookAt)
	{
		// 카메라의 전방 벡터를 계산
		Eigen::Vector3f cameraFront = data.cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = data.cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * currInput.Lstick.Y + right * currInput.Lstick.X;

		movementDirection.y() = 0.f;

		// 속도 벡터를 계산
		Eigen::Vector3f movement = movementDirection * _moveSpeed * currInput.Lstick.Size;

		Eigen::Vector3f velocity = data.playerRigidbody->GetLinearVelocity();

		// y축 운동을 방해하지 않는 걸로 중력은 적용되도록함.
		/// 단, 이런 방식이면 키 입력이 없다면 누군가랑 부딪쳐도 가만히 있을 듯
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// 속력을 적용시킴
		data.playerRigidbody->SetLinearVelocity(velocity);

		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();

		{
			static bool lastbool = false;
			static bool currentbool = false;

			lastbool = currentbool;
			currentbool = movementDirection.isZero();

			if (lastbool != currentbool)
			{
				if (currentbool)
				{
					data.animator->Play(L"Armature|Armature|Armature|idle");
				}
				else
				{
					data.animator->Play(L"Armature|Armature|Armature|running");
				}
				return;
			}

			if (currentbool)
			{
				return;
			}

			data.animator->SetPlaySpeed(L"Armature|Armature|Armature|running", currInput.Lstick.Size);
		}

		if (_cameraLookAt)
		{
			const Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

			// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
			const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

			// Set the rotation of the transform to the target rotation
			data.modelCore->SetLocalRotation(targetRotation);
		}
	}

	void Player::CameraCoreUpdate()
	{
		auto currP = data.cameraCore->GetLocalPosition();

		// 각도에 따라 카메라 위치를 움직임.
		if (data.xAngle > 0.f)
		{
			const float ease = data.xAngle / data.limitHighAngle;
			auto EasingHigh = [](float x) -> float
				{
					return 1.0f - (1.0f - x) * (1.0f - x);
				};

			currP.z() = data.coreDefaultPosition.z() + differenceHigh.z() * EasingHigh(ease);

			data.cameraCore->SetLocalPosition(currP);
		}
		else if (data.xAngle < 0.f)
		{
			const float ease = data.xAngle / data.limitLowAngle;
			auto EasingLow = [](float x) -> float
				{
					return 1.0f - std::pow(1.0f - x, 5.0f);
				};

			currP.z() = data.coreDefaultPosition.z() + differenceLow.z() * EasingLow(ease);
			data.cameraCore->SetLocalPosition(currP);
		}
	}

	bool Player::CameraUpdate()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		data.acclerpFactor += deltaTime;

		if (data.acclerpFactor < 1.f)
		{
			const float t = data.acclerpFactor / data.lerpFactor;

			auto coreP = lerp(data.coreStartPosition, data.coreTargetPosition, t);
			auto coreR = slerp(data.coreStartRotation, data.coreTargetRotation, t);

			auto armP = lerp(data.armStartPosition, data.armTargetPosition, t);
			auto armR = slerp(data.armStartRotation, data.armTargetRotation, t);

			data.cameraCore->SetLocalPosition(coreP);
			data.cameraCore->SetLocalRotation(coreR);

			std::wstring str0 = std::to_wstring(coreP.x()) +
				L" " + std::to_wstring(coreP.y()) +
				L" " + std::to_wstring(coreP.z());

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				str0, 1000, 600, 100, 100, 20, 255, 255, 255, 255);

			data.cameraArm->SetLocalPosition(armP);
			data.cameraArm->SetLocalRotation(armR);

			std::wstring str1 = std::to_wstring(armP.x()) +
				L" " + std::to_wstring(armP.y()) +
				L" " + std::to_wstring(armP.z());

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				str1, 1000, 700, 100, 100, 20, 255, 255, 255, 255);

			return false;
		}
		else
		{
			data.acclerpFactor = 1.f;

			data.cameraUpdate = false;

			return true;
		}
	}

	void Player::CameraReset()
	{
		data.coreStartPosition = data.cameraCore->GetLocalPosition();
		data.coreStartRotation = data.cameraCore->GetLocalRotation();

		data.coreTargetPosition = data.coreDefaultPosition;
		data.coreTargetRotation = data.coreDefaultRotation;

		data.armStartPosition =	data.cameraArm->GetLocalPosition();
		data.armStartRotation =	data.cameraArm->GetLocalRotation();

		data.armTargetPosition = data.armDefaultPosition;
		data.armTargetRotation = data.modelCore->GetLocalRotation();

		data.acclerpFactor = 0.f;
		data.cameraUpdate = true;

		data.xAngle = 0.f;
	}

	void Player::CameraAround()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// 스틱 기울기에 따라 회전 각도를 계산
		const float yawAngle = currInput.Rstick.X * data.sensitivity * deltaTime * currInput.Rstick.Size;
		{
			// 월드 up 기준으로 카메라를 회전
			data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// 스틱 기울기에 따라 회전 각도를 계산
		const float pitchAngle = -currInput.Rstick.Y * data.sensitivity * deltaTime * currInput.Rstick.Size;
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

			// 카메라 Right 벡터를 기준으로 회전하기 위해서 카메라의 월드 right를 구함.
			const Eigen::Vector3f cameraRight = data.cameraArm->GetWorldRotation() * Eigen::Vector3f::UnitX();

			// 카메라의 right 기준으로 카메라를 회전
			data.cameraArm->Rotate(cameraRight, deltaAngle);
		}

		CameraCoreUpdate();
	}

	bool Player::CameraForwardRaycast()
	{
		const Eigen::Vector3f from = data.cameraCore->GetWorldPosition();
		const Eigen::Vector3f to = data.cameraCore->GetWorldRotation().toRotationMatrix() * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		ZonaiPhysics::ZnRaycastInfo info;

		const bool block = PurahEngine::Physics::Raycast(from, to, 1000.f, info);

		if (block)
		{
			PurahEngine::Collider* shape = static_cast<PurahEngine::Collider*>(info.colliderData);

			if (shape == nullptr)
			{
				return false;
			}

			float distance = info.distance;
			Eigen::Vector3f hitPosition = info.position;

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				shape->GetGameObject()->GetName(), 800, 600, 100, 100, 30, 255, 255, 255, 255);
		}

		return block;
	}

	void Player::ChangeState(State _state)
	{
		currState = _state;
	}

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

		auto holder = data.holder;
		POSTDESERIALIZE_PTR(holder);
		data.holder = holder;
	}
}
