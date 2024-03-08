#include "InputManager.h"

PurahEngine::InputManager::InputManager(): state(), result(0), hWnd(nullptr)
{
}

PurahEngine::InputManager::~InputManager()
= default;

void PurahEngine::InputManager::Initialize(HWND hwnd, eKey* _inputArr, UINT _size)
{
	hWnd = hwnd;

	for (auto i = 0; i < _size; i++)
	{
		key.push_back(_inputArr[i]);
	}

	PrevKeyState.clear();
	NowKeyState.clear();
}

void PurahEngine::InputManager::Update()
{
	if (hWnd == GetFocus())
	{
		for (const auto e : key)
		{
			PrevKeyState[e] = NowKeyState[e];
			NowKeyState[e] = GetAsyncKeyState(static_cast<int>(e));
		}
	}
}

// 키를 방금 눌렀는가
bool PurahEngine::InputManager::IsKeyDown(eKey keycode)
{
	return (PrevKeyState[keycode] == 0) && (NowKeyState[keycode] & 0x8001);
}

// 키를 눌리고 있는가
bool PurahEngine::InputManager::IsKeyPressed(eKey keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] & 0x8001);
}

// 키를 뗐는가
bool PurahEngine::InputManager::IsKeyUp(eKey keycode)
{
	return (PrevKeyState[keycode] & 0x8001) && (NowKeyState[keycode] == 0);
}

PurahEngine::InputManager& PurahEngine::InputManager::Getinstance()
{
	static InputManager instance;
	return instance;
}