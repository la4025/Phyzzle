#include "Timer.h"

namespace PurahEngine
{
	std::queue<ITimer*> Timer::addTimer;
	std::queue<ITimer*> Timer::removeTimer;
	std::vector<ITimer*> Timer::handleTimer;

	void Timer::Update()
	{
		// 새 타이머 추가
		while (!addTimer.empty())
		{
			handleTimer.push_back(addTimer.front());
			addTimer.pop();
		}

		// 타이머 제거
		while (!removeTimer.empty())
		{
			auto timer = removeTimer.front();
			auto iter = std::ranges::find(handleTimer, timer);
			if (iter != handleTimer.end())
			{
				handleTimer.erase(iter);
				delete timer; // 메모리 누수 방지
			}
			removeTimer.pop();
		}

		// 타이머 업데이트
		for (const auto timer : handleTimer)
			timer->Update();
	}
}
