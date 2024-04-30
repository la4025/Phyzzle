#include <algorithm>
#include <iostream>

#include "IState.h"
#include "DefaultState.h"
#include "AttachHoldState.h"
#include "AttachSelectState.h"
#include "RewindState.h"
#include "LockState.h"

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

	void Player::Start()
	{
		gamePad = PurahEngine::GamePadManager::GetGamePad(0);

		data.armDefaultPosition = data.cameraArm->GetLocalPosition();
		data.armDefaultRotation = data.cameraArm->GetLocalRotation();

		data.coreDefaultPosition = data.cameraCore->GetLocalPosition();
		data.coreDefaultRotation = data.cameraCore->GetLocalRotation();

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

		stateSystem.insert(std::make_pair(ATTACH_HOLD, new AttachHoldState(this)));
		stateSystem.insert(std::make_pair(DEFAULT, new DefaultState(this)));

		stateSystem.insert(std::make_pair(ATTACH_SELECT, new AttachSelectState(this)));
		stateSystem.insert(std::make_pair(REWIND_SELECT, new RewindState(this)));
		stateSystem.insert(std::make_pair(LOCK_SELECT, new LockState(this)));

		stateChange.insert(ATTACH_SELECT);
		stateChange.insert(REWIND_SELECT);
		stateChange.insert(LOCK_SELECT);
	}

	void Player::Update()
	{
		prevState = currState;

		DebugDraw();
		GamePadInput();

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
			if (data.state == ATTACH_HOLD)
			{
				str = L"ATTACH_HOLD";
			}
			else if (data.state == ATTACH_SELECT)
			{
				str = L"ATTACH_SELECT";
			}
			else if (data.state == REWIND_SELECT)
			{
				str = L"REWIND_SELECT";
			}
			else if (data.state == LOCK_SELECT)
			{
				str = L"LOCK_SELECT";
			}
			else
			{
				str = L"ERROR";
			}
		}

		std::wstring str0{};
		{
			if (currState == ATTACH_HOLD)
			{
				str0 = L"ATTACH_HOLD";
			}
			else if (currState == ATTACH_SELECT)
			{
				str0 = L"ATTACH_SELECT";
			}
			else if (currState == REWIND_SELECT)
			{
				str0 = L"REWIND_SELECT";
			}
			else if (currState == LOCK_SELECT)
			{
				str0 = L"LOCK_SELECT";
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

		if (!data.jumping)
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"can Jump",
				500, 200,
				400, 200, 15,
				255, 255, 255, 255);
		}
		else
		{
			PurahEngine::GraphicsManager::GetInstance().DrawString(
				L"Jumping",
				500, 200,
				400, 200, 15,
				255, 255, 255, 255);
		}
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
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_SHOULDER_L))
				stateSystem[currState]->Pressing_LB();
			else if (gamePad->IsKeyUp(PurahEngine::ePad::ePAD_SHOULDER_L))
				stateSystem[currState]->Up_LB();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_SHOULDER_R))
				stateSystem[currState]->Click_RB();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_SHOULDER_R))
				stateSystem[currState]->Pressing_RB();
			else if (gamePad->IsKeyUp(PurahEngine::ePad::ePAD_SHOULDER_R))
				stateSystem[currState]->Up_RB();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_A))
				stateSystem[currState]->Click_A();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_A))
				stateSystem[currState]->Pressing_A();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_B))
				stateSystem[currState]->Click_B();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_B))
				stateSystem[currState]->Pressing_B();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_X))
				stateSystem[currState]->Click_X();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_X))
				stateSystem[currState]->Pressing_X();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_Y))
				stateSystem[currState]->Click_Y();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_Y))
				stateSystem[currState]->Pressing_Y();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_UP))
				stateSystem[currState]->Click_DUp();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_UP))
				stateSystem[currState]->Pressing_DUp();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_DOWN))
				stateSystem[currState]->Click_DDown();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_DOWN))
				stateSystem[currState]->Pressing_DDown();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_LEFT))
				stateSystem[currState]->Click_DLeft();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_LEFT))
				stateSystem[currState]->Pressing_DLeft();

			if (gamePad->IsKeyDown(PurahEngine::ePad::ePAD_RIGHT))
				stateSystem[currState]->Click_DRight();
			else if (gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_RIGHT))
				stateSystem[currState]->Pressing_DRight();

		}
	}

	void Player::Jump()
	{
		if (!data.jumping)
		{
			Eigen::Vector3f power = Eigen::Vector3f::UnitY()* data.jumpPower;
			data.playerRigidbody->AddForce(power, ZonaiPhysics::ForceType::Force);
			data.jumping = true;
		}
	}

	void Player::JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		const auto velo = zn_collision.thisPostLinearVelocity;

		const Eigen::Vector3f up{ 0.f, 1.f, 0.f };
		const Eigen::Vector3f direction{ velo };
		float cosTheta1 = up.dot(direction);
		cosTheta1 = std::clamp(cosTheta1, -1.f, 1.f);

		if ((cosTheta1 >= 0.5f))
		{
			data.jumping = false;
		}
	}

	void Player::PlayerMove(float _moveSpeed)
	{
		// 카메라의 전방 벡터를 계산
		const Eigen::Vector3f cameraFront = data.cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = data.cameraArm->GetRight();

		const Eigen::Vector3f movementDirection = forward * currInput.Lstick.Y + right * currInput.Lstick.X;

		// 속도 벡터를 계산
		const Eigen::Vector3f movement = movementDirection * _moveSpeed * currInput.Lstick.Size;

		Eigen::Vector3f velocity = data.playerRigidbody->GetLinearVelocity();

		// y축 운동을 방해하지 않는 걸로 중력은 적용되도록함.
		/// 단, 이런 방식이면 키 입력이 없다면 누군가랑 부딪쳐도 가만히 있을 듯
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// 속력을 적용시킴
		data.playerRigidbody->SetLinearVelocity(velocity);

		/// 애니메이션 부분은 고칠 필요가 있음.
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

	void Player::CameraReset()
	{
		//data.coreStartPosition = data.cameraCore->GetLocalPosition();
		//data.coreStartRotation = data.cameraCore->GetLocalRotation();

		//data.coreTargetPosition = data.coreDefaultPosition;
		//data.coreTargetRotation = data.coreDefaultRotation;

		//data.armStartPosition =	data.cameraArm->GetLocalPosition();
		//data.armStartRotation =	data.cameraArm->GetLocalRotation();

		//data.armTargetPosition = data.armDefaultPosition;
		//data.armTargetRotation = data.modelCore->GetLocalRotation();

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

	bool Player::CameraForwardRaycast(float _distance,
		PurahEngine::RigidBody** _outBody, float* _outDistance, Eigen::Vector3f* _outHitPosition)
	{
		const Eigen::Vector3f from = data.cameraCore->GetWorldPosition();
		const Eigen::Vector3f to = data.cameraCore->GetWorldRotation().toRotationMatrix() * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		ZonaiPhysics::ZnRaycastInfo info;

		const bool block = PurahEngine::Physics::Raycast(from, to, _distance, info);

		if (block)
		{
			const PurahEngine::Collider* shape = static_cast<PurahEngine::Collider*>(info.colliderData);

			if (!shape)
				return false;

			PurahEngine::RigidBody* body = shape->GetGameObject()->GetComponent<PurahEngine::RigidBody>();

			if (!body)
				return false;

			if (_outBody)
				*_outBody = body;
			if (_outDistance)
				*_outDistance = info.distance;
			if (_outHitPosition)
				*_outHitPosition = info.position;

			PurahEngine::GraphicsManager::GetInstance().DrawString(
				shape->GetGameObject()->GetName(), 800, 600, 100, 100, 30, 255, 255, 255, 255);
		}

		return block;
	}

	void Player::ChangeState(State _state)
	{
		currState = _state;
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

		//auto holder = data.holder;
		//POSTDESERIALIZE_PTR(holder);
		//data.holder = holder;
	}
#pragma endregion 직렬화
}
