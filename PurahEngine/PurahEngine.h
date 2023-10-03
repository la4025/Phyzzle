#pragma once
/// 외부에는 이부분만 보여지게 하기 위하도록 설계

#include "PurahEngineAPI.h"

#include "GameLoop.h"

namespace PurahEngine
{
	PURAHENGINE_API void Initialize();
	PURAHENGINE_API void Run();
	PURAHENGINE_API void Finalize();
};

