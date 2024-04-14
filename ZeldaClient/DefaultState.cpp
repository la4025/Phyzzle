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

	void DefaultState::Input()
	{
		IState::Input();


	}

	void DefaultState::StateEnter()
	{

	}

	void DefaultState::StateExit()
	{

	}

	void DefaultState::operator()()
	{

	}

	void DefaultState::Stick_L()
	{
		Move();
	}

	void DefaultState::Stick_R()
	{
		Around();
	}

	void DefaultState::Trigger_L()
	{
		
	}

	void DefaultState::Trigger_R()
	{
	}

	void DefaultState::Click_DUp()
	{
	}

	void DefaultState::Click_DDown()
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

	void DefaultState::Click_B()
	{

	}

	void DefaultState::Click_X()
	{
	}

	void DefaultState::Click_Y()
	{
	}

	void DefaultState::Click_LB()
	{
		player->ChangeState(player->data.state);
	}

	void DefaultState::Click_RB()
	{
		// 리버레코 취소
	}

	void DefaultState::Push() const
	{

	}

	void DefaultState::Hold() const
	{
		auto target = player->data.holder->GetHolderableBody();
		if (target)
		{
			// player->ChangeState(Player::State::HOLD);
		}
	}

	void DefaultState::Jump() const
	{
		player->Jump();
	}

	void DefaultState::Move() const
	{
		player->Move(player->data.moveSpeed, true);
	}

	void DefaultState::Around() const
	{
		player->CameraAround();
	}

	void DefaultState::Select() const
	{
		player->CameraForwardRaycast();
	}

	// 현재 능력을 변경함
	void DefaultState::ChangeState(bool _value) const
	{
		if (_value)
		{
			Player::State newState = 
				static_cast<Player::State>(
					(player->data.state + 1) 
					% (player->stateSystem.size() - 1)
					);

			player->data.state = newState;
		}
		else
		{
			Player::State newState = 
				static_cast<Player::State>(
					max(static_cast<int>(player->data.state - 1), 0)
					% (player->stateSystem.size() - 1)
					); 

			player->data.state = newState;
		}
	}
}
