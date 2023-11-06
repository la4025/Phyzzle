#include "include/Platform.h"
#include "OsPlatform.h"
#include "../FloaterRendererCommon/include/IRenderer.h"
#include <chrono>

class flt::Platform::impl : public PLATFORM_OS
{

};

flt::Platform::Platform()
{
	_pOsImpl = new impl;
}

flt::Platform::~Platform()
{
	delete _pOsImpl;
}


bool flt::Platform::Initialize(int pixelWidth, int pixelHeight, std::wstring title, std::wstring imgPath)
{
	return _pOsImpl->Initialize(pixelWidth, pixelHeight, title, imgPath);
}

bool flt::Platform::Finalize()
{
	return _pOsImpl->Finalize();
}

bool flt::Platform::Update()
{
	return _pOsImpl->Update();
}

flt::IRenderer* flt::Platform::CreateRenderer(RendererType type)
{
	return _pOsImpl->CreateRenderer(type);
}

void flt::Platform::DestroyRenderer(IRenderer* renderer)
{
	_pOsImpl->DestroyRenderer(renderer);
}

flt::KeyData flt::Platform::GetKey(KeyCode code)
{
	return _pOsImpl->GetKey(code);
}

flt::KeyData flt::Platform::GetGamePad(int playerNum)
{
	return _pOsImpl->GetGamePad(playerNum);
}
