#include <iostream>
#include <cassert>

// dll import 확인용 (나중에 수정 할 것)
#include "../PurahEngine/PurahEngine.h"

//#pragma comment(lib, "../x64/Debug/PurahEngine.lib")

//using namespace PurahEngine;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";
	
	std::string path = "..//x64//Debug//PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	if (PurahEngineDll == nullptr)
	{
		assert(0);
	}

	auto createPurahEngine = reinterpret_cast<PurahEngine* (*)()>(GetProcAddress(PurahEngineDll, "CreateZeldaRenderer"));
	if (createPurahEngine == nullptr)
	{
		// DLL 함수를 찾을 수 없습니다.
		assert(0);
	}

	PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080);
	PurahEngine::Run();
	PurahEngine::Finalize();
}

//int main()
//{
//	using namespace std;
//	cout << "Zelda Project Start!!" << endl;
//
//	return 0;
//}