#include "EasingFunc.h"
#include "TimeController.h"
#include <algorithm>
#include <iostream>

#include "PhysicsSystem.h"

#include "Player.h"

#include "DefaultState.h"
#include "IState.h"

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
		stateSystem.insert(std::make_pair(State::HOLD, nullptr));
		stateSystem.insert(std::make_pair(State::ATTATCH, nullptr));
		stateSystem.insert(std::make_pair(State::REWIND, nullptr));
	}

	void Player::Update()
	{
		GamePadInput();

		(*stateSystem[state])();
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

	void Player::JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider)
	{
		const float velo{ zn_collision.thisPostLinearVelocity.y() };

		const Eigen::Vector3f up{ 0.f, 1.f, 0.f };
		// const Eigen::Vector3f direction{ zn_collision.impulses.normalized() };
		const Eigen::Vector3f direction{ velo };
		float cosTheta1 = up.dot(direction);
		cosTheta1 = std::clamp(cosTheta1, -1.f, 1.f);
		OutputDebugStringW(std::wstring(std::to_wstring(cosTheta1) + L'\n').c_str());

		if ((cosTheta1 >= 0.5f))
		{
			data.jumping = false;
		}
	}

	void Player::GamePadInput()
	{
		pad.LstickX = 0.f, pad.LstickY = 0.f;
		pad.RstickX = 0.f, pad.RstickY = 0.f;
		pad.LTrigger = 0.f, pad.RTrigger = 0.f;

		if (pad.gamePad->IsConnected())
		{
			pad.LstickSize = pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_L, pad.LstickX, pad.LstickY);
			pad.RstickSize = pad.gamePad->GetStickRatio(PurahEngine::ePadStick::ePAD_STICK_R, pad.RstickX, pad.RstickY);

			if (pad.LstickSize != 0.f)
				stateSystem[state]->Stick_L();
			if (pad.RstickSize != 0.f)
				stateSystem[state]->Stick_R();

			pad.LTrigger = pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_L);
			pad.RTrigger = pad.gamePad->GetTriggerRatio(PurahEngine::ePadTrigger::ePAD_TRIGGER_R);

			if (pad.LTrigger != 0.f)
				stateSystem[state]->Trigger_L();
			if (pad.RTrigger != 0.f)
				stateSystem[state]->Trigger_R();

			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_A))
				stateSystem[state]->Click_A();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_B))
				stateSystem[state]->Click_B();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_X))
				stateSystem[state]->Click_X();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_Y))
				stateSystem[state]->Click_Y();

			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_UP))
				stateSystem[state]->Click_DUp();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_DOWN))
				stateSystem[state]->Click_DDown();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_LEFT))
				stateSystem[state]->Click_DLeft();
			if (pad.gamePad->IsKeyDown(PurahEngine::ePad::ePAD_RIGHT))
				stateSystem[state]->Click_DRight();
		}
	}

	void Player::ChangeState(State _state)
	{
		state = _state;
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
