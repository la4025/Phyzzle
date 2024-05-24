#pragma once
#include <string>

namespace PurahEngine
{
	class Transform;

	enum class SoundType
	{
		BGM,
		EFFECT
	};

	struct PurahSound
	{
	public:
		std::wstring soundName;
		Transform* transform;
		SoundType type;
		bool isPlaying;
		float mMinDistance;
		float mMaxDistance;
	};
}