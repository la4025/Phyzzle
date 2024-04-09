#define _CRT_SECURE_NO_WARNINGS

#include "GameLoop.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PhysicsSystem.h"
#include "TimeController.h"
#include "Timer.h"
#include "SoundManager.h"
#include "EngineSetting.h"
#include <cassert>


/// <summary>
///  임시
/// </summary>
#include "GamePadManager.h"
#include "../ZonaiPhysicsBase/ZnCollider.h"
#include "../ZonaiPhysicsBase/ZnRigidBody.h"
#include "../ZonaiMath/ZonaiMath.h"

ZonaiPhysics::ZnCollider* collider;
ZonaiPhysics::ZnRigidBody* rigidBody;

static std::string timeInit = "Init";

PurahEngine::GameLoop::GameLoop()
{

}

PurahEngine::GameLoop::~GameLoop()
{

}

void PurahEngine::GameLoop::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	// 내가 쓸 윈도우를 등록
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // 프로시저
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = gameName;
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	// 그 윈도우를 생성
	hWnd = CreateWindowW(gameName, gameName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

#ifdef _DEBUG
	{
		// 콘솔창 생성
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
#endif

	SetMenu(hWnd, NULL);

	// Engine Setting 초기화 (GraphicsManager, Physics Manager의 초기화보다 먼저 되어야 한다.)
	EngineSetting::GetInstance().Initialize();

	// 시간관리자 초기화
	PurahEngine::TimeController::GetInstance().Initialize(timeInit);

	// Graphics dll 초기화(변경 가능성 농후)
	PurahEngine::GraphicsManager::GetInstance().Initialize(hWnd);

	// InputManager 초기화
	eKey key[] =
	{
		eKey::eKEY_W,
		eKey::eKEY_A,
		eKey::eKEY_S,
		eKey::eKEY_D,
		eKey::eKEY_O,
		eKey::eKEY_P,
		eKey::eKEY_Q,
		eKey::eKEY_E,

		eKey::eKEY_LEFT,
		eKey::eKEY_RIGHT,
		eKey::eKEY_UP,
		eKey::eKEY_DOWN,

		eKey::eKEY_ESCAPE,
		eKey::eKEY_SHIFT,
		eKey::eKEY_LSHIFT,
		eKey::eKEY_0,
		eKey::eKEY_1,
		eKey::eKEY_2,

		eKey::eKEY_CONTROL,
		eKey::eKEY_F1,
		eKey::eKEY_F2,
		eKey::eKEY_F3,
		eKey::eKEY_F4,
		eKey::eKEY_F5,
		eKey::eKEY_F6,
		eKey::eKEY_F7,
		eKey::eKEY_F8,

		eKey::eKEY_F11,
		eKey::eKEY_F12
	};
	PurahEngine::InputManager::Getinstance().Initialize(hWnd, key, sizeof(key) / sizeof(eKey));
	PurahEngine::GamePadManager::Instance().Initialize(hWnd, nullptr, 0);

	// SceneManager 초기화
	PurahEngine::SceneManager::GetInstance().Initialize();

	// PhysicsSysyem 초기화
	PurahEngine::PhysicsSystem::GetInstance().Initialize();

	// SoundManager 초기화
	PurahEngine::SoundManager::GetInstance().Initialize();
}

void PurahEngine::GameLoop::Run(_In_ int nCmdShow)
{
	// 윈도우 핸들이 가장 중요
	if (!hWnd)
	{
		return;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 무한루프 (안에 메시지 루프와 게임 루프 등이 포함됨)
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			DispatchMessage(&msg);
		}
		else
		{
			// 엔진 동작
			run();
		}
	}
}

void PurahEngine::GameLoop::Finalize()
{
	GraphicsManager::GetInstance().Finalize();
	PhysicsSystem::GetInstance().Finalize();
	GamePadManager::Instance().Release();

#ifdef _DEBUG
	FreeConsole();
#endif
}

void PurahEngine::GameLoop::run()
{
	TimeController::GetInstance().Update(timeInit);

	Timer::PreUpdate();
	const float deltaTime = TimeController::GetInstance().GetDeltaTime(timeInit);

	PhysicsSystem::GetInstance().PreStep();
	PhysicsSystem::GetInstance().Simulation(deltaTime);

	InputManager::Getinstance().Update();
	GamePadManager::Instance().Update();

	Timer::Update();
	if (InputManager::Getinstance().IsKeyPressed(eKey::eKEY_SHIFT) && InputManager::Getinstance().IsKeyDown(eKey::eKEY_ESCAPE))
	{
		SceneManager::GetInstance().LoadScene(L"DataExportTestWorldObjectInfo.json");
	}

	SceneManager::GetInstance().LoadScene();
	SceneManager::GetInstance().InitializationEvent();
	SceneManager::GetInstance().DecommissionEvent();

	SceneManager::GetInstance().Update();
	SoundManager::GetInstance().Update();

	GraphicsManager::GetInstance().UpdateAnimator(deltaTime);
	GraphicsManager::GetInstance().Render(deltaTime);
}

LRESULT CALLBACK PurahEngine::GameLoop::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_ENTERSIZEMOVE:
		case WM_MOVE:
		case WM_EXITSIZEMOVE:
		{
			TimeController::GetInstance().MoveWindow();
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

PurahEngine::GameLoop& PurahEngine::GameLoop::GetInstance()
{
	static GameLoop instance;
	return instance;
}
