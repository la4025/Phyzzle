#include <Eigen/Dense>

#include "Transform.h"
#include "TimeController.h"
#include "Player.h"

#include "DefaultState.h"

#include "Holder.h"

namespace Phyzzle
{
	DefaultState::~DefaultState()
	= default;

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
	}

	void DefaultState::Click_DRight()
	{
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
		player->ChangeState(Player::State::ATTATCH);
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
}
