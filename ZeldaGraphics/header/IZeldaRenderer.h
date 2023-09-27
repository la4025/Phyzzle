#pragma once

#include "ZeldaGraphicsAPI.h"

#include <Windows.h>

class ZELDAGRAPHICS_API IZeldaRenderer
{
public:
	virtual bool Initialize(HWND hwnd) abstract;

	virtual void BeginDraw() abstract;
	virtual void EndDraw() abstract;
};