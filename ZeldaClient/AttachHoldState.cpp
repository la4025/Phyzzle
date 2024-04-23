#include "AttachHoldState.h"

namespace Phyzzle
{
	AttachHoldState::~AttachHoldState()
	= default;

	void AttachHoldState::StateEnter()
	{

	}

	void AttachHoldState::StateExit()
	{

	}

	void AttachHoldState::StateStay()
	{

	}

	void AttachHoldState::Stick_L()
	{

	}

	void AttachHoldState::Stick_R()
	{

	}

	void AttachHoldState::Click_DUp()
	{

	}

	void AttachHoldState::Click_DDown()
	{

	}

	void AttachHoldState::Click_DLeft()
	{
	}

	void AttachHoldState::Click_DRight()
	{
	}

	void AttachHoldState::Pressing_DUp()
	{
		IState::Pressing_DUp();
	}

	void AttachHoldState::Pressing_DDown()
	{
		IState::Pressing_DDown();
	}

	void AttachHoldState::Pressing_DLeft()
	{
		IState::Pressing_DLeft();
	}

	void AttachHoldState::Pressing_DRight()
	{
		IState::Pressing_DRight();
	}

	void AttachHoldState::Pressing_RB()
	{
		IState::Pressing_RB();
	}

	void AttachHoldState::Click_A()
	{
	}

	void AttachHoldState::Click_B()
	{
	}

	void AttachHoldState::Click_X()
	{
	}

	void AttachHoldState::Click_Y()
	{
	}

	void AttachHoldState::Click_LB()
	{
		Cancel();
	}

	void AttachHoldState::Cancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	void AttachHoldState::XRotate() const
	{
		
	}

	void AttachHoldState::YRotate() const
	{

	}
}
