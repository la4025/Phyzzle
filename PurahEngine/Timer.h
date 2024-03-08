#pragma once
#include <queue>

namespace PurahEngine
{
	class ITimer
	{
	public:
		void Update();
	};

	class Timer
	{
	public:
		static void Update();
		static void Delay();

	private:
		static std::queue<ITimer*> addTimer;
		static std::queue<ITimer*> removeTimer;
		static std::vector<ITimer*> handleTimer;
	};
}

