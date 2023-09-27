#pragma once

#include "ZeldaGraphicsAPI.h"

class IZeldaRenderer;

namespace ZeldaGraphics
{
	extern "C" ZELDAGRAPHICS_API IZeldaRenderer * CreateZeldaRenderer();
}