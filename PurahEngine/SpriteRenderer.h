#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API SpriteRenderer final : public Renderer
	{
	public:
		void Render(IZeldaRenderer* renderer) override;
		
		void SetTexture(const std::wstring& fileName);

	private:
		std::wstring textureName;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}