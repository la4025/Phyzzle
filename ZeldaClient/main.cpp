#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"


#include "TestMovement.h"
#include "CameraMovement.h"
#include "TestSingleton.h"
#include "AudioTest.h"

#include "Player.h"
#include "Controller.h"

#include "LauncherPlatform.h"

#include "TriggerDevice.h"
#include "ButtonDevice.h"
#include "DisconnectedWire.h"
#include "Door.h"
#include "DoorParts.h"
#include "ElectricWire.h"
#include "SceneLoadSupport.h"
#include "CameraEventDevice.h"
#include "ShockDetector.h"

#include "Burnable.h"
#include "ObjectGenerator.h"
#include "RespawnTrigger.h"
#include "RespawnSystem.h"

/// 각자 만드는 것들
#include "PzObject.h"
#include "NaDong.h"
#include "BeomJoon.h"
#include "Holder.h"
#include "KiHoon.h"
#include "Rewindable.h"
#include "YongHun.h"

namespace PurahEngine
{
	class BoxCollider;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
// int APIENTRY main(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL 함수를 찾을 수 없습니다.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test>("Test");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test2>("Test2");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::Test3>("Test3");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::TestMovement>("TestMovement");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::CameraMovement>("CameraMovement");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<AnimationTestController>("AnimationTestController");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<PurahEngine::AudioTest>("AudioTest");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<LauncherPlatform>("LauncherPlatform");

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Controller>("Controller");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Player>("Player");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Holder>("Holder");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::PzObject>("PzObject");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Rewindable>("Rewindable");

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::TriggerDevice>("TriggerDevice");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::ButtonDevice>("ButtonDevice");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::DisconnectedWire>("DisconnectedWire");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Door>("Door");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::DoorParts>("DoorParts");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::ElectricWire>("ElectricWire");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::SceneLoadSupport>("SceneLoadSupport");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::CameraEventDevice>("CameraEventDevice");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::ShockDetector>("ShockDetector");

	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::Burnable>("Burnable");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::ObjectGenerator>("ObjectGenerator");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::RespawnTrigger>("RespawnTrigger");
	PurahEngine::ComponentFactory::GetInstance().RegisterComponent<Phyzzle::RespawnSystem>("RespawnSystem");

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); 역할
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
