#include "Timer.h"
#include "TimerHandler.h"

void PurahEngine::TimerHandler::Update()
{
	accumulation += 0.016f;

	if (accumulation <= delay)
	{
		callback();

		if (loop)
		{
			accumulation = 0.f;
		}
		else
		{
			Timer::Cancel(this);
		}
	}
}