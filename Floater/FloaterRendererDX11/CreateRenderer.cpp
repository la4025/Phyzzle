#include "./include/CreateRenderer.h"
#include "RendererDX11.h"

extern "C" flt::IRenderer* flt::CreateRendererDX11(HWND hwnd)
{
	RendererDX11* renderer = new RendererDX11();
	if (!renderer->Initialize(hwnd))
	{
		renderer->Finalize();
		delete renderer;
		return nullptr;
	}
	return renderer;
}

extern "C" flt::IRenderer* flt::CreateRendererDX12(HWND hwnd)
{
	return nullptr;
}
