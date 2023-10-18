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

bool PurahEngine::InputManager::IsKeyPressedNow(int keycode)
{
	return (PrevKeyState[keycode] & 0x0000) && (NowKeyState[keycode] & 0x8000);
}

bool PurahEngine::InputManager::IsKeyPressed(int keycode)
{
	return (PrevKeyState[keycode] & 0x8000) && (NowKeyState[keycode] & 0x8001);
}

bool PurahEngine::InputManager::IsKeyReleasedNow(int keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] & 0x0000);
}

bool PurahEngine::InputManager::IskeyRelease(int keycode)
{
	return (PrevKeyState[keycode] & 0x0000) && (NowKeyState[keycode] & 0x0000);
}

PurahEngine::InputManager& PurahEngine::InputManager::Getinstance()
{
	static InputManager instance;
	return instance;
}
