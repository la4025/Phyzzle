#include "PurahEngine.h"
#include "GameLoop.h"

_In_ int nCmdShow;


void PurahEngine::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)
{
	PurahEngine::GameLoop::GetInstance().Initialize(hInstance, gameName, width, height);
}

void PurahEngine::Run(_In_ int nCmdShow)
{
	PurahEngine::GameLoop::GetInstance().Run(nCmdShow);
}

void PurahEngine::Finalize()
{

}
