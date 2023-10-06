#pragma once

#include "ZeldaGraphicsAPI.h"

#include <Windows.h>

class ZELDAGRAPHICS_API IZeldaRenderer
{
public:
	virtual void CreateResources() abstract;

	virtual bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear) abstract;

	virtual void BeginDraw() abstract;
	virtual void EndDraw() abstract;

	virtual void DrawCube() abstract;
};