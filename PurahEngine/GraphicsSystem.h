#pragma once

#include "PurahEngineAPI.h"

#include <windows.h>
#include <Eigen/dense>

#include "GraphicsResourceID.h"

 
#ifdef YH_RENDERER
#include "../Floater/ZeldaGraphicsAdapter/include/IZeldaRendererAdapter.h"
#else
#include "IZeldaRenderer.h"
#endif

#include "ZeldaGraphics.h"
#include "ZeldaGraphicsAPI.h"

namespace PurahEngine
{
	class Camera;
	class Renderer;

	class PURAHENGINE_API GraphicsSystem
	{
	public:
		void Initialize(HWND hWnd);

		void Render(float _dt);


		void AddRenderer(Renderer* render);
		void RemoveRenderer(Renderer* render);

		IZeldaRenderer* GetRenderer();

	private:
		HMODULE zeldaGraphicsDLL;
		IZeldaRenderer* renderer;

		std::vector<Renderer*> rendererList;

	private:
		GraphicsSystem();
		~GraphicsSystem();
		GraphicsSystem(const GraphicsSystem& ref) = delete;
		GraphicsSystem& operator=(const GraphicsSystem& ref) = delete;

	public:
		static GraphicsSystem& GetInstance();
	};

}

