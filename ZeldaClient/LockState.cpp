#include "LockState.h"


namespace Phyzzle
{
	void LockState::StateEnter()
	{

	}

	void LockState::StateExit()
	{

	}

	void LockState::StateStay()
	{

	}

	void LockState::Click_LB()
	{
		Cancel();
	}

	void LockState::Cancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}
}
