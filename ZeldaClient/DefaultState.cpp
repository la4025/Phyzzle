#include <Eigen/Dense>
#include <cmath>
#include <iostream>

#include "Transform.h"
#include "TimeController.h"
#include "Player.h"

#include "DefaultState.h"

#include "Holder.h"

namespace Phyzzle
{
	DefaultState::~DefaultState()
	= default;

	void DefaultState::StateEnter()
	{
		player->data.holdObject = nullptr;
		player->data.holdObjectBody = nullptr;
	}

	void DefaultState::StateExit()
	{}

	void DefaultState::StateStay()
	{
		
	}

	void DefaultState::PostStateStay()
	{
		CameraUpdate();
	}

	void DefaultState::StateCancel()
	{}

	void DefaultState::Stick_L()
	{
		Move();

		auto direction = player->data.playerRigidbody->GetLinearVelocity();
		direction.y() = 0.f;
		LookToWorldDirection(direction);
	}

	void DefaultState::Stick_R()
	{

	}

	void DefaultState::Click_DLeft()
	{
		ChangeState(false);
	}

	void DefaultState::Click_DRight()
	{
		ChangeState(true);
	}

	void DefaultState::Click_A()
	{
		Jump();
	}

	void DefaultState::Click_LB()
	{
		if (!player->data.jumping)
		{
			player->ChangeAbilityState(player->data.state);
		}
	}

	void DefaultState::Jump() const
	{
		if (player->TryJump())
		{
			player->ChangePlayerAnimationState(Player::JUMP);
		}
	}

	void DefaultState::CameraUpdate()
	{
		player->UpdateDefaultCamera();
	}

	void DefaultState::Move() const
	{
		if (player->TryPlayerMove(player->data.moveSpeed))
		{
			if (player->currInput.Lstick.Size > 0.75f)
			{
				player->ChangePlayerAnimationState(Player::RUN);
			}
			else
			{
				player->ChangePlayerAnimationState(Player::WALK);
			}
		}
		else
		{
			player->ChangePlayerAnimationState(Player::IDLE);
		}
	}

	void DefaultState::Around() const
	{
		player->RotateCameraArm();
	}

	void DefaultState::LookToWorldDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInWorldDirection(_to);
	}

	void DefaultState::LookToLocalDirection(const Eigen::Vector3f& _to) const
	{
		player->LookInLocalDirection(_to);
	}

	// 현재 능력을 변경함
	void DefaultState::ChangeState(bool _value) const
	{
		Player::AbilityState newState = Player::AbilityState::DEFAULT;

		const int size = player->stateChange.size() + 1;

		if (_value)
		{
			newState =
				static_cast<Player::AbilityState>(
					(player->data.state + 1)
					);

			newState = 
				static_cast<Player::AbilityState>(
					max(newState % size, 1)
					);

			if (newState == Player::AbilityState::DEFAULT)
			{
				newState = static_cast<Player::AbilityState>(
					newState + 1
					);
			}
		}
		else
		{

			newState =
				static_cast<Player::AbilityState>(
					player->data.state - 1
					);

			if (newState == Player::AbilityState::DEFAULT)
			{
				newState = static_cast<Player::AbilityState>(
					size - 1
					);
			}
		}

		player->data.state = newState;
	}
}
