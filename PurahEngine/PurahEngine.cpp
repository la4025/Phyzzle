#include "PurahEngine.h"
#include "GameLoop.h"

_In_ int nCmdShow;


void PurahEngine::Initialize()
{
	PurahEngine::GameLoop::GetInstance().Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height);
}

void PurahEngine::Run()
{
	PurahEngine::GameLoop::GetInstance().Run(nCmdShow);
}

void PurahEngine::Finalize()
{

}
