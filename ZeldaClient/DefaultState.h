#pragma once
#include "IState.h"
#include "Coroutine.h"

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
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
		void Stick_L() override;
		void Stick_R() override;

		void Click_DLeft() override;
		void Click_DRight() override;

		void Pressing_DLeft() override;
		void Pressing_DRight() override;

		void Click_A() override;

		void Pressing_A() override;

		void Click_LB() override;
		void Click_RB() override;

		void Pressing_LB() override;
		void Pressing_RB() override;

		Coroutine<void> CameraTemp();
		Coroutine<int> CameraTempint();

	private:
		void Jump() const;
		void CameraUpdate();
		void CameraAround();
		void CameraPositionFromAngle();
		void Move() const;
		void Around() const;
		void LookToWorldDirection(const Eigen::Vector3f& _to) const;
		void LookToLocalDirection(const Eigen::Vector3f& _to) const;
		void ChangeState(bool _value) const;
	};
}
