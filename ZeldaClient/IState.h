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
		virtual void Stick_L() = 0;
		virtual void Stick_R() = 0;

		virtual void Trigger_L() = 0;
		virtual void Trigger_R() = 0;

		virtual void Click_DUp() = 0;
		virtual void Click_DDown() = 0;
		virtual void Click_DLeft() = 0;
		virtual void Click_DRight() = 0;

		virtual void Click_A() = 0;
		virtual void Click_B() = 0;
		virtual void Click_X() = 0;
		virtual void Click_Y() = 0;

		virtual void Click_LB() = 0;
		virtual void Click_RB() = 0;

	public:
		virtual void operator()() = 0;
	};
}
