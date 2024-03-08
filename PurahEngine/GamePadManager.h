#pragma once
#include "GamePad.h"
#include <windows.h>

namespace PurahEngine
{
	class GamePad;

	class GamePadManager
	{
	private:
		GamePadManager();
		~GamePadManager();
		GamePadManager& operator=(const GamePadManager&) = delete;
		GamePadManager&& operator=(GamePadManager&&) noexcept = delete;

	public:
		void Initialize(HWND _hwnd, ePad* _keyArr, int _size);
		void Update() const;

	private:
		HWND hWnd;
		GamePad pad[4];

	public:
		static GamePadManager& Instance();
		static void Release();
	};
}

