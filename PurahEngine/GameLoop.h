#pragma once
#include <windows.h>

namespace PurahEngine
{
	class GameLoop
	{
	public:
		void Initialize();
		void Run();
		void Finalize();
	};
}

