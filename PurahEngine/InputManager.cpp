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

}

bool PurahEngine::InputManager::IsKeyPressed(int keycode)
{

}

bool PurahEngine::InputManager::IsKeyReleasedNow(int keycode)
{

}

bool PurahEngine::InputManager::IskeyRelease(int keycode)
{

}

PurahEngine::InputManager& PurahEngine::InputManager::Getinstance()
{
	static InputManager instance;
	return instance;
}
