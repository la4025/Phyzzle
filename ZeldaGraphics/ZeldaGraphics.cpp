#include "ZeldaGraphics.h"

#include "ZeldaDX11Renderer.h"

namespace ZeldaGraphics
{
	IZeldaRenderer* CreateZeldaRenderer()
	{
		return new ZeldaDX11Renderer();
	}
}