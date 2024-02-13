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

	class PURAHENGINE_API GraphicsManager
	{
	public:
		void Initialize(HWND hWnd);

		void Run();


		void AddRenderer(Renderer* render);
		void RemoveRenderer();

		IZeldaRenderer* GetRenderer();

	private:
		HMODULE zeldaGraphicsDLL;
		IZeldaRenderer* renderer;

		std::vector<Renderer*> rendererList;

	private:
		GraphicsManager();
		~GraphicsManager();
		GraphicsManager(const GraphicsManager& ref) = delete;
		GraphicsManager& operator=(const GraphicsManager& ref) = delete;

	public:
		static GraphicsManager& GetInstance();
	};

}

