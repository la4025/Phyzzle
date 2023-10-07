#include <iostream>


// dll import 확인용 (나중에 수정 할 것)
#include "../PurahEngine/PurahEngine.h"

#pragma comment(lib, "../x64/Debug/PurahEngine.lib")

//using namespace PurahEngine;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";



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