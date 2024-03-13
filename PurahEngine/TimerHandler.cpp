#include "TimeController.h"

#include "Timer.h"
#include "TimerHandler.h"

void PurahEngine::TimerHandler::Update()
{
	accumulation += TimeController::GetInstance().GetDeltaTime("Simulate");
	// accumulation += 0.1f;

	if (std::fabs(delay - accumulation) <= timerEpsilon)
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