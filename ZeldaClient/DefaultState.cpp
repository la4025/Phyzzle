#include <Eigen/Dense>
#include <cmath>

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

	}

	void DefaultState::StateExit()
	{

	}

	void DefaultState::StateStay()
	{

	}

	void DefaultState::Stick_L()
	{
		Move();

		auto direction = player->data.playerRigidbody->GetLinearVelocity();
		direction.y() = 0.f;
		LookToWorldDirection(direction);
	}

	void DefaultState::Stick_R()
	{
		Around();
	}

	void DefaultState::Click_DLeft()
	{
		ChangeState(false);
	}

	void DefaultState::Click_DRight()
	{
		ChangeState(true);
	}

	void DefaultState::Pressing_DLeft()
	{

	}

	void DefaultState::Pressing_DRight()
	{

	}

	void DefaultState::Pressing_A()
	{

	}

	void DefaultState::Pressing_LB()
	{

	}

	void DefaultState::Pressing_RB()
	{

	}

	void DefaultState::Click_A()
	{
		Jump();
	}

	void DefaultState::Click_LB()
	{
		player->ChangeState(player->data.state);
	}

	void DefaultState::Click_RB()
	{
		// player->CameraReset();
	}

	void DefaultState::Jump() const
	{
		player->Jump();
	}

	void DefaultState::Move() const
	{
		player->PlayerMove(player->data.moveSpeed);
	}

	void DefaultState::Around() const
	{
		player->CameraAround();
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
		Player::State newState = Player::State::DEFAULT;

		const int size = player->stateChange.size() + 1;

		if (_value)
		{
			newState =
				static_cast<Player::State>(
					(player->data.state + 1)
					);

			newState = 
				static_cast<Player::State>(
					max(newState % size, 1)
					);

			if (newState == Player::State::DEFAULT)
			{
				newState = static_cast<Player::State>(
					newState + 1
					);
			}
		}
		else
		{

			newState =
				static_cast<Player::State>(
					player->data.state - 1
					);

			if (newState == Player::State::DEFAULT)
			{
				newState = static_cast<Player::State>(
					size - 1
					);
			}
		}

		player->data.state = newState;
	}
}
