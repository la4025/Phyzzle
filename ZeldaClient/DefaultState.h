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
		void Input() override;
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
		void Stick_L() override;
		void Stick_R() override;

		void Click_DLeft() override;
		void Click_DRight() override;
		
		void Click_A() override;
		
		void Click_LB() override;
		void Click_RB() override;

	private:
		void Jump() const;
		void Move() const;
		void Around() const;
		void LookToWorldDirection(const Eigen::Vector3f& _to) const;
		void LookToLocalDirection(const Eigen::Vector3f& _to) const;
		void ChangeState(bool _value) const;
	};
}
