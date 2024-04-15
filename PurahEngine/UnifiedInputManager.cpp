#include "UnifiedInputManager.h"

namespace PurahEngine
{
	bool UnifiedInputManager::IsKeyDown(eUnfInput keycode)
	{
		return false;
	}
	bool UnifiedInputManager::IsKeyPressed(eUnfInput keycode)
	{
		return false;
	}
	bool UnifiedInputManager::IsKeyUp(eUnfInput keycode)
	{
		return false;
	}
	UnifiedInputManager& UnifiedInputManager::Getinstance()
	{
		static UnifiedInputManager instance;
		return instance;
	}

	UnifiedInputManager::UnifiedInputManager()
	{

	}

	UnifiedInputManager::~UnifiedInputManager()
	{

	}
}