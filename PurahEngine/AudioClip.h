#pragma once
#include "SoundManager.h"

namespace PurahEngine
{
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
		FMOD::ChannelGroup* group;
		SoundType type;
		FMOD_VECTOR lastPos;
		bool isPlaying;
	};

	class SoundManager;

	class AudioClip
	{
	public:
		AudioClip();
		~AudioClip();

		void Play();
		void Stop();
		void Set3DAttributes();
		void SetLoop();


	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool isLoop;

	private:
		SoundManager& soundManager = SoundManager::GetInstance();

	};
}