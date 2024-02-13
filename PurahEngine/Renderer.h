#pragma once
#include "PurahEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API Renderer : public Component, IRenderer
	{
	public:
		Renderer();
		~Renderer() override;
		Renderer(const Renderer&) = default;
		Renderer(Renderer&&) noexcept = default;
		Renderer& operator=(const Renderer&) = default;
		Renderer& operator=(Renderer&&) noexcept = default;

		void Render(IZeldaRenderer* renderer) override;

		void AddTexture(TextureID textureID);
		void AddLight(LightID lightID);
	private:
		TextureID texture;
		LightID light = LightID::ID_NULL;
	};
}

