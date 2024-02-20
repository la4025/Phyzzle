#pragma once

#include "Renderer.h"

namespace PurahEngine
{
	class Animator;

	class PURAHENGINE_API ModelRenderer final : public Renderer
	{
	public:
		~ModelRenderer();

		void Initialize() override;

		// Renderer을(를) 통해 상속됨
		void Render(IZeldaRenderer* renderer) override;

		void SetModelName(const std::wstring& modelName);

	private:
		Animator* animator;
		
		std::wstring modelName;
		
		bool isBlending;
		std::wstring animationName1;
		std::wstring animationName2;
		float time1;
		float time2;
		float ratio;


		friend Animator;
	};
}