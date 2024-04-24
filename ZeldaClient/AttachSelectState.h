#pragma once
#include "IState.h"
#include "Coroutine.h"

namespace Phyzzle
{
	class AttachSelectState final : public IState
	{
	public:
		AttachSelectState() = delete;
		explicit AttachSelectState(Player* _player)
			: IState(_player)
		{}
		~AttachSelectState() override;

#pragma region StateEvent
	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;
#pragma endregion StateEvent

#pragma region Input
	private:
		void Stick_L() override;
		void Stick_R() override;

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_LB() override;

		void Pressing_RB() override;
#pragma endregion Input

#pragma region Content
	private:
		bool select = false;

	private:
		void PlayerMove(float _speed) const;
		void CameraAround() const;
		void StateCancel() const;

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to);
		void LookToLocalDirection(const Eigen::Vector3f& _to);

		bool Search() const;
#pragma endregion Content
		
#if _DEBUG
	private:
		void SearchDebugDraw(bool _value);
#endif _DEBUG
	};
}

