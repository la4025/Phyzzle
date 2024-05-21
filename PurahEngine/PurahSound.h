#pragma once
#include <fmod.hpp>
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
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		SoundType type;
		float minDistance;
		float maxDistance;
		bool isPlaying;
		bool isLoop;
	};
}