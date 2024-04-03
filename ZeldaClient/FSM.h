#pragma once
#include <functional>
#include <unordered_map>


namespace Phyzzle
{
	class FSM
	{
	public:
		FSM();
		~FSM();

	private:
		std::unordered_map<int, std::function<void()>> stateMap;
		int prevState;
		int currState;

	public:
		int Excute()
		{
			return 0;
		}


	};
}
