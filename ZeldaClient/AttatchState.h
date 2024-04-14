#pragma once
#include "IState.h"

namespace Phyzzle
{
	class AttatchState final : public IState
	{
	public:
		AttatchState() = delete;
		explicit AttatchState(Player* _player)
			: IState(_player)
		{}
		~AttatchState() override;

	private:
		void Input() override;
		void StateEnter() override;
		void StateExit() override;
		void operator()() override;

	private:
		void Stick_L() override;
		void Stick_R() override;
		void Trigger_L() override;
		void Trigger_R() override;
		void Click_DUp() override;
		void Click_DDown() override;
		void Click_DLeft() override;
		void Click_DRight() override;
		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;
		void Click_LB() override;
		void Click_RB() override;
	};
}

