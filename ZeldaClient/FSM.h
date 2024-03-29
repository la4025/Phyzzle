#pragma once
#include <functional>
#include <unordered_map>


namespace Phyzzle
{
	template <typename Type, typename Enum>
	class FSM
	{
	private:
		friend typename Type;

		// 상태와 함수만 엮어놓음
		std::unordered_map<Enum, std::function<Enum()>> stateFunc;

	public:
		FSM() : currState() {}

		Enum currState;

		void Add(Enum _state, std::function<Enum()> _func) noexcept
		{
			stateFunc.insert(std::make_pair(_state, _func));
		}

		void Excute() noexcept
		{
			currState = stateFunc[currState]();
		}
	};
}
