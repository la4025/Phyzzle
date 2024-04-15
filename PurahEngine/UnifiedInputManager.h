#pragma once

#include "PurahEngineAPI.h"
#include "eUnfInput.h"

namespace PurahEngine
{
	class PURAHENGINE_API UnifiedInputManager final
	{
	public:
		// 키가 방금 눌렸는가?
		bool IsKeyDown(eUnfInput keycode);
		// 키를 눌리고 있는가?
		bool IsKeyPressed(eUnfInput keycode);
		// 키를 방금 뗐는가?
		bool IsKeyUp(eUnfInput keycode);

	private:


	public:
		static UnifiedInputManager& Getinstance();

	private:
		UnifiedInputManager();
		~UnifiedInputManager();
		UnifiedInputManager(const UnifiedInputManager& ref) = delete;
		UnifiedInputManager& operator=(const UnifiedInputManager& ref) = delete;
	};
}