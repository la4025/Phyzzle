#pragma once
#include "IState.h"

namespace Phyzzle
{
	class DefaultState final : public IState
	{
	public:
		DefaultState() = delete;
		explicit DefaultState(Player* _player)
			: IState(_player)
		{}
		~DefaultState() override;

	private:
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

	private:
		void Push() const;
		void Hold() const;
		void Jump() const;
		void Move() const;
		void Around() const;
		void Select() const;
	};
}
