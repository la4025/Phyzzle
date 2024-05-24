#pragma once
#include "SoundManager.h"
#include "PurahSound.h"

namespace PurahEngine
{
	class SoundManager;

	class AudioClip
	{
	public:
		AudioClip();
		~AudioClip();

		void CreateSound(PurahSound newSound);
		void Play(PurahSound newSound);
		void Stop();
		void Set3DAttributes(FMOD_VECTOR pos);
		void SetLoop();


	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;

	private:
		SoundManager& soundManager = SoundManager::GetInstance();

	};
}