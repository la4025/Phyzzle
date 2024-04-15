#include "TimeController.h"

const float PurahEngine::TimeController::TIMESCALE_DEFAULT = 1.0f;
const int PurahEngine::TimeController::FPS_DEFAULT = 60;



float PurahEngine::TimeController::GetDeltaTime(std::string timeName)
{
	if (isRunning == false)
	{
		return 0.0f;
	}

	return deltaTime[timeName];
}

void PurahEngine::TimeController::GetEventDeltaTime(std::string timeName)
{

}

float PurahEngine::TimeController::GetFPS()
{
	return static_cast<float>(framePerSecond);
}

void PurahEngine::TimeController::SetTimeScale(float timescale)
{
	timeScale = timescale;
}

float PurahEngine::TimeController::GetTimeScale()
{
	return timeScale;
}

float PurahEngine::TimeController::GetDeltaTime()
{
	return GetDeltaTime(timeName);
}

void PurahEngine::TimeController::Initialize(std::string timename)
{
	timeScale = TIMESCALE_DEFAULT;
	framePerSecond = FPS_DEFAULT;
	timeName = timename;
	isRunning = true;
}

void PurahEngine::TimeController::Update(std::string timeName)
{
	if (isRunning == false)
	{
		return;
	}

	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

	if (timeTable.count(timeName) == 0)
	{
		timeTable[timeName] = currentTime;
	}

	std::chrono::duration<float>sec = currentTime - timeTable[timeName];
	deltaTime[timeName] = sec.count() * timeScale;
	eventDeltaTime[timeName] = sec.count();

	timeTable[timeName] = currentTime;
}

void PurahEngine::TimeController::UpdateIgnoreDelta(std::string timeName)
{
	if (isRunning == false)
	{
		return;
	}

	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

	if (timeTable.count(timeName) == 0)
	{
		timeTable[timeName] = currentTime;
	}

	timeTable[timeName] = currentTime;
}

void PurahEngine::TimeController::PauseAll()
{
	if (isRunning == false)
	{
		return;
	}

	isRunning = false;

	pauseTime = std::chrono::system_clock::now();
}

void PurahEngine::TimeController::ResumeAll()
{
	if (isRunning == true)
	{
		return;
	}

	isRunning = true;

	std::chrono::system_clock::time_point resumeTime = std::chrono::system_clock::now();
	std::chrono::system_clock::duration pauseDuration = resumeTime - pauseTime;

	for (auto iter = timeTable.begin(); iter != timeTable.end(); iter++)
	{
		iter->second += pauseDuration;
	}
}

void PurahEngine::TimeController::MoveWindow()
{
	for (auto iter = timeTable.begin(); iter != timeTable.end(); iter++)
	{
		UpdateIgnoreDelta((*iter).first);
	}
}

PurahEngine::TimeController& PurahEngine::TimeController::GetInstance()
{
	static TimeController instance;
	return instance;
}

PurahEngine::TimeController::TimeController()
{

}

PurahEngine::TimeController::~TimeController()
{

}
