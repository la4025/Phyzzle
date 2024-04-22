#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class UI;

	class PURAHENGINE_API Image : public Renderer
	{
	public:
		void Awake() override;

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetTexture(const std::wstring& fileName);

	private:
		UI* ui;

	private:
		int layer;
		std::wstring textureName;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}