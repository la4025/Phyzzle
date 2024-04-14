#include <algorithm>
#include <iostream>

#include "IState.h"
#include "DefaultState.h"
#include "HoldState.h"
#include "AttatchState.h"
#include "RewindState.h"

#include "Player.h"

namespace Phyzzle
{
	Player::~Player()
	{
		
	}

	void Player::Start()
	{
		PurahEngine::GamePadManager::AddGamePad(0);

		pad.gamePad = PurahEngine::GamePadManager::GetGamePad(0);
		pad.gamePad->SetDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		startPosition = data.cameraCore->GetLocalPosition();
		startRotation = data.cameraCore->GetLocalRotation();

		highPosition = Eigen::Vector3f(0.f, 0.f, -20.f);
		lowPosition = Eigen::Vector3f(0.f, 0.f, -2.f);
		differenceHigh = highPosition - startPosition;
		differenceLow = lowPosition - startPosition;

		stateSystem.insert(std::make_pair(State::DEFAULT, new DefaultState(this)));
		stateSystem.insert(std::make_pair(State::HOLD, new HoldState(this)));
		stateSystem.insert(std::make_pair(State::ATTATCH, new AttatchState(this)));
		stateSystem.insert(std::make_pair(State::REWIND, new RewindState(this)));
	}

	void Player::Update()
	{
		prevState = currState;

		stateSystem[currState]->Input();

		if (prevState != currState)
		{
			stateSystem[prevState]->StateExit();
			stateSystem[currState]->StateEnter();
		}

		(*stateSystem[currState])();
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

	void Player::GamePadInput()
	{
		pad.Lstick.X = 0.f, pad.Lstick.Y = 0.f;
		pad.Rstick.X = 0.f, pad.Rstick.Y = 0.f;
		pad.LTrigger = 0.f, pad.RTrigger = 0.f;

		if (pad.gamePad->IsConnected())
		{
			pad.Lstick.Size = pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, pad.Lstick.X, pad.Lstick.Y);
			pad.Rstick.Size = pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, pad.Rstick.X, pad.Rstick.Y);

			stateSystem[currState]->Stick_L();
			stateSystem[currState]->Stick_R();

			pad.LTrigger = pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			pad.RTrigger = pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

			stateSystem[currState]->Trigger_L();
			stateSystem[currState]->Trigger_R();

			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_SHOULDER_L))
				stateSystem[currState]->Click_LB();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_SHOULDER_R))
				stateSystem[currState]->Click_RB();

			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_A))
				stateSystem[currState]->Click_A();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_B))
				stateSystem[currState]->Click_B();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_X))
				stateSystem[currState]->Click_X();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_Y))
				stateSystem[currState]->Click_Y();

			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_UP))
				stateSystem[currState]->Click_DUp();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_DOWN))
				stateSystem[currState]->Click_DDown();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_LEFT))
				stateSystem[currState]->Click_DLeft();
			if (pad.gamePad->IsKeyPressed(PurahEngine::ePad::ePAD_RIGHT))
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

		Eigen::Vector3f movementDirection = forward * pad.Lstick.Y + right * pad.Lstick.X;

		movementDirection.y() = 0.f;

		// 속도 벡터를 계산
		Eigen::Vector3f movement = movementDirection * _moveSpeed * pad.Lstick.Size;

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

			data.animator->SetPlaySpeed(L"Armature|Armature|Armature|running", pad.Lstick.Size);
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

	void Player::CameraAround()
	{
		PurahEngine::TimeController& time = PurahEngine::TimeController::GetInstance();

		const float deltaTime = time.GetDeltaTime();

		// 스틱 기울기에 따라 회전 각도를 계산
		const float yawAngle = pad.Rstick.X * data.sensitivity * deltaTime * pad.Rstick.Size;
		{
			// 월드 up 기준으로 카메라를 회전
			data.cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);
		}

		// 스틱 기울기에 따라 회전 각도를 계산
		const float pitchAngle = -pad.Rstick.Y * data.sensitivity * deltaTime * pad.Rstick.Size;
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

			// 각도에 따라 카메라 위치를 움직임.
			if (data.xAngle > 0.f)
			{
				const float ease = data.xAngle / data.limitHighAngle;
				auto EasingHigh = [](float x) -> float
					{
						return 1.0f - (1.0f - x) * (1.0f - x);
					};
				data.cameraCore->SetLocalPosition(startPosition + differenceHigh * EasingHigh(ease));
			}
			else if (data.xAngle < 0.f)
			{
				const float ease = data.xAngle / data.limitLowAngle;
				auto EasingLow = [](float x) -> float
					{
						return 1.0f - std::pow(1.0f - x, 5.0f);
					};
				data.cameraCore->SetLocalPosition(startPosition + differenceLow * EasingLow(ease));
			}
			else
			{
				data.cameraCore->SetLocalPosition(startPosition);
			}
		}
	}

	void Player::CameraForwardRaycast()
	{
		const Eigen::Vector3f from = data.cameraCore->GetWorldPosition();
		const Eigen::Vector3f to = data.cameraCore->GetWorldRotation().toRotationMatrix() * Eigen::Vector3f{ 0.f, 0.f, 1.f };
		ZonaiPhysics::ZnRaycastInfo info;

		const bool block = PurahEngine::Physics::Raycast(from, to, 1000.f, info);

		if (block)
		{
			PurahEngine::RigidBody* body = static_cast<PurahEngine::RigidBody*>(info.bodyData);
			PurahEngine::Collider* shape = static_cast<PurahEngine::Collider*>(info.colliderData);

			if (body == nullptr && shape == nullptr)
			{
				return;
			}

			Component* com = nullptr;
			if (body)
			{
				com = body;
			}
			else
			{
				com = shape;
			}

			float distance = info.distance;
			Eigen::Vector3f hitPosition = info.position;

			std::wcout << L"selectName : " << com->GetGameObject()->GetName() << std::endl;
			std::wcout << L"distance : " << distance << std::endl;
			std::wcout << L"hitPosition : " << hitPosition.x() << " " << hitPosition.y() << " " << hitPosition.z() << std::endl;
		}
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
