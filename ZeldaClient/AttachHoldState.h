#pragma once
#include "IState.h"

namespace Phyzzle
{
	class AttachHoldState final : public IState
	{
	public:
		AttachHoldState() = delete;
		explicit AttachHoldState(Player* _player)
			: IState(_player)
		{}
		~AttachHoldState() override;

	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
#pragma region Input
		void Stick_L() override;
		void Stick_R() override;

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_DUp() override;
		void Click_DDown() override;
		void Click_DLeft() override;
		void Click_DRight() override;

		void Pressing_DUp() override;
		void Pressing_DDown() override;
		void Pressing_DLeft() override;
		void Pressing_DRight() override;

		void Click_LB() override;

		void Pressing_RB() override;
#pragma endregion Input

	private:
		bool rotateMode = false;

	private:
		void Cancel() const;
		void XRotate() const;
		void YRotate() const;
	};
}