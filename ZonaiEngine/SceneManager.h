#pragma once
#include "BaseDefine.h"

namespace ZonaiEngine
{
	class Scene;

	class SceneManager
	{
	public:
		static Scene* CreateScene() noexcept;
		static void LoadScene(Scene*) noexcept;
		static void LoadScene(const std::wstring&) noexcept;
		static void UnloadScene() noexcept;

	private:
		static std::map<std::wstring, Scene*> sceneList;
	};
}

