#include "AttatchState.h"


namespace Phyzzle
{
	AttatchState::~AttatchState()
		= default;

	void AttatchState::Input()
	{

	}

	void AttatchState::StateEnter()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p += Eigen::Vector3f{ 1.f ,1.f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);
	}

	void AttatchState::StateExit()
	{
		auto p = player->data.cameraCore->GetLocalPosition();
		p -= Eigen::Vector3f{ 1.f ,1.f, 0.f };

		player->data.cameraCore->SetLocalPosition(p);
	}

	void AttatchState::operator()()
	{

	}

	void AttatchState::Stick_L()
	{
		Move();
	}

	void AttatchState::Stick_R()
	{
		Around();
	}

	void AttatchState::Click_A()
	{
		Cancel();
	}

	void AttatchState::Click_B()
	{
		if (ObjectSelect())
		{
			selected = true;
		}
	}

	void AttatchState::Click_X()
	{
		Cancel();
	}

	void AttatchState::Click_Y()
	{
		Cancel();
	}

	void AttatchState::Click_LB()
	{
		Cancel();
	}

	void AttatchState::Click_RB()
	{
		Cancel();
	}

	void AttatchState::Move() const
	{
		player->Move(player->data.moveSpeed, true);
	}

	void AttatchState::Around() const
	{
		player->CameraAround();
	}

	void AttatchState::Cancel() const
	{
		player->ChangeState(Player::State::DEFAULT);
	}

	bool AttatchState::ObjectSelect() const
	{
		return  player->CameraForwardRaycast();
	}

	void AttatchState::Jump() const
	{
		player->Jump();
	}
}
