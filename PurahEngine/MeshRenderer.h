#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class PURAHENGINE_API MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();

		enum class MeshType : int
		{
			None = 0,
			Cube = 1
		};

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetTexture(const std::wstring& textureName);
		void SetMesh(MeshType type);
		void SetWireFrame(bool value);
		void SetColor(float r, float g, float b, float a);
		void SetColor(Color color);

	private:
		MeshType meshType;
		bool wireFrame;
		std::wstring textureName;

		Color meshColor;
	};
}