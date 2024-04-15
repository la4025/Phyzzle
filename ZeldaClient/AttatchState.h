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

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_LB() override;
		void Click_RB() override;

	private:
		bool selected = false;

		void Move() const;
		void Around() const;
		void Cancel() const;
		bool ObjectSelect() const;
		void Jump() const;
	};
}

