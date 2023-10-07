#pragma once

#ifdef PURAHENGINE_EXPORTS
	#define PURAHENGINE_API extern "C" __declspec(dllexport)
#else
	#define PURAHENGINE_API extern "C" __declspec(dllimport)
#endif