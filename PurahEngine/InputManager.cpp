#include "InputManager.h"

PurahEngine::InputManager::InputManager()
{

}

PurahEngine::InputManager::~InputManager()
{

}

void PurahEngine::InputManager::Update()
{
	for (int i = 0; i < KEY_COUNT; i++)
	{
		PrevKeyState[i] = NowKeyState[i];
		NowKeyState[i] = GetAsyncKeyState(i);
	}
}

bool PurahEngine::InputManager::IsKeyDown(int keycode)
{
	return (PrevKeyState[keycode] == 0) && (NowKeyState[keycode] & 0x8001);
}

bool PurahEngine::InputManager::IsKeyPressed(int keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] & 0x8001);
}

bool PurahEngine::InputManager::IsKeyUp(int keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] == 0);
}


PurahEngine::InputManager& PurahEngine::InputManager::Getinstance()
{
	static InputManager instance;
	return instance;
}
