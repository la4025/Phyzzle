#pragma once
#include "PurahEngineAPI.h"

#include "PurahSound.h"

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <unordered_map>

namespace PurahEngine
{
	class Transform;
	class AudioSource;

	class PURAHENGINE_API SoundManager
	{
	public:
		void Initialize();
		void Finalize();

		void LoadSound(PurahSound newSound, FMOD::Sound** sound);
		
		void ReleaseSound(AudioSource* audioSource);

		void LoadBGMSound(PurahSound newSound, FMOD::Sound** sound);

		void LoadSfxSound(PurahSound newSound, FMOD::Sound** sound);

		void Play(PurahSound newSound, FMOD::Sound* sound, FMOD::Channel** channel);

		void PlayBGM(FMOD::Sound* sound, FMOD::Channel** channel);

		void PlaySfx(FMOD::Sound* sound, FMOD::Channel** channel);

		void Update();

		void Set3DListenerAttributes(FMOD_VECTOR pos, FMOD_VECTOR forward, FMOD_VECTOR up);

		FMOD::System* GetSystem() const;

	private:
		FMOD::System* system;
		FMOD::Channel* bgmChannel;
		FMOD::Channel* effectChannel;
		FMOD::ChannelGroup* effectChannelGroup;

	private:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager& ref) = delete;
		// 클래스를 생성하게 될 경우, 기본적으로 = 에 대한 연산자 오버로딩이 생성된다.
		// 싱글턴은 객체가 하나여야 하므로 그것을 방지하기 위해, 명시적으로 delete를 사용하여 사용을 막는다.
		SoundManager& operator=(const SoundManager& ref) = delete;

	public:
		static SoundManager& GetInstance();
	};
}

