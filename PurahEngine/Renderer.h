#pragma once
#include "IZeldaRenderer.h"
#include "Component.h"

namespace PurahEngine
{
	class Renderer : public Component, IZeldaRenderer
	{
	public:
		Renderer();
		~Renderer();

	public:
		virtual void Render(IZeldaRenderer* graphics) abstract;
	};
}

