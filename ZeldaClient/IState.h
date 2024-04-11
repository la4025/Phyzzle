#pragma once

namespace Phyzzle
{
	class Player;

	class IState
	{
	public:
		IState() = delete;
		IState(Player* _player) : player(_player) {}
		virtual ~IState() = default;

	protected:
		Player* player;

	public:
		virtual void Stick_L() {}
		virtual void Stick_R() {}

		virtual void Trigger_L() {}
		virtual void Trigger_R() {}

		virtual void Click_DUp() {}
		virtual void Click_DDown() {}
		virtual void Click_DLeft() {}
		virtual void Click_DRight() {}

		virtual void Click_A() {}
		virtual void Click_B() {}
		virtual void Click_X() {}
		virtual void Click_Y() {}

		virtual void Click_LB() {}
		virtual void Click_RB() {}

	public:
		virtual void operator()() {}
	};
}
