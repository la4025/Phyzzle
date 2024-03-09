#include "Timer.h"
#include "PreTimerHandler.h"

void PurahEngine::PreTimerHandler::Update()
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
