#include "./include/IZeldaRendererAdapter.h"

bool IZeldaRendererAdapter::Initialize(unsigned int screenWidth, unsigned int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float cameraNear)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_vsync = vsync;
	_hwnd = hwnd;
	_fullScreen = fullScreen;
	_screenDepth = screenDepth;
	_cameraNear = cameraNear;

	_renderer = flt::CreateRendererDX11(hwnd);

	if (_renderer == nullptr)
	{
		return false;
	}

	return true;
}

void IZeldaRendererAdapter::Finalize()
{
	flt::DestroyRendererDX11(_renderer);
}

void IZeldaRendererAdapter::BeginDraw()
{

}

void IZeldaRendererAdapter::EndDraw()
{
	_renderer->Render();
}
