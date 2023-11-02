#pragma once

#include "ZeldaGraphicsAPI.h"

class IZeldaRenderer;

namespace ZeldaGraphics
{
	extern "C" ZELDAGRAPHICS_API IZeldaRenderer* CreateZeldaRenderer();
	extern "C" ZELDAGRAPHICS_API void ReleaseZeldaRenderer(IZeldaRenderer* renderer);
}