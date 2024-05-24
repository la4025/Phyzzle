#pragma once
#include "IState.h"

namespace Phyzzle
{
	class LockState final : public IState
	{
	public:
		LockState() = delete;
		explicit LockState(Player* _player)
			: IState(_player)
		{}
		~LockState() override {}

	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
		void Click_LB() override;

#pragma region Content
	private:
		PurahEngine::RigidBody* result = nullptr;
		PzObject* pzObject = nullptr;

	private:
		bool Aim();
		void Cancel() const;
#pragma endregion Content
	};
}