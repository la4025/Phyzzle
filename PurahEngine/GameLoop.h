#pragma once
#include <windows.h>

namespace PurahEngine
{
	class GameLoop
	{
	public:
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
		// 외부에서 사용할 Run() 함수
		void Run(_In_ int nCmdShow);
		void Finalize();

	private:
		HWND hWnd;
		// 내부적으로 Run()함수에서 돌아갈 함수
		void run();

		//
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

