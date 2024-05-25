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

	void LockState::Click_B()
	{
		IState::Click_B();
	}

	void LockState::Click_LB()
	{
		Cancel();
	}

	bool LockState::Aim()
	{
		const float distance = 40.f;
		const bool hit = player->RaycastFromCamera(distance, &result, &pzObject, nullptr);

		if (!hit)
			return false;

		if (result->IsKinematic())
			return false;

		if (result->GetGameObject()->tag.IsContain(L"Phyzzle Player"))
			return false;

		return true;
	}

	void LockState::Select()
	{

	}

	void LockState::Cancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}
}
