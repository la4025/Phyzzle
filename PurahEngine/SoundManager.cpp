#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "SoundManager.h"
#include "Transform.h"

#include <cassert>
#include <locale>
#include <codecvt>

void PurahEngine::SoundManager::Initialize()
{
	FMOD_RESULT result;

	// Create the main system object.
	result = FMOD::System_Create(&system);

	assert(result == FMOD_OK);

	// Initialize FMOD.
	result = system->init(100, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	result = system->set3DSettings(1.0, 1.0, 1.0);
}

void PurahEngine::SoundManager::Finalize()
{
	system->release();
}

void PurahEngine::SoundManager::LoadSound(PurahSound newSound, FMOD::Sound** sound)
{
	switch (newSound.type)
	{
		case SoundType::BGM:
		{
			LoadBGMSound(newSound, sound);
			break;
		}
		case SoundType::EFFECT:
		{
			LoadSfxSound(newSound, sound);
			break;
		}
	}
}

void PurahEngine::SoundManager::ReleaseSound(AudioSource* audioSource)
{

}

void PurahEngine::SoundManager::LoadBGMSound(PurahSound newSound, FMOD::Sound** sound)
{
	FMOD_RESULT result;
	std::wstring filePath = L"../Sound/BGM/Test/" + newSound.soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::LoadSfxSound(PurahSound newSound, FMOD::Sound** sound)
{
	FMOD_RESULT result;
	std::wstring filePath = L"../Sound/SFX/Test/" + newSound.soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_3D_LINEARSQUAREROLLOFF, 0, sound);
	assert(result == FMOD_OK);

	(*sound)->set3DMinMaxDistance(newSound.mMinDistance, newSound.mMaxDistance);
}

void PurahEngine::SoundManager::Play(PurahSound newSound, FMOD::Sound* sound, FMOD::Channel** channel)
{
	switch (newSound.type)
	{
		case SoundType::BGM:
		{
			PlayBGM(sound, channel);
			break;
		}
		case SoundType::EFFECT:
		{
			PlaySfx(sound, channel);
			break;
		}
	}
}

void PurahEngine::SoundManager::PlayBGM(FMOD::Sound* sound, FMOD::Channel** channel)
{
	bool isBGMPlaying = true;
	(*channel)->isPlaying(&isBGMPlaying);

	if (isBGMPlaying)
	{
		(*channel)->stop();
	}

	system->playSound(sound, 0, false, channel);
}

void PurahEngine::SoundManager::PlaySfx(FMOD::Sound* sound, FMOD::Channel** channel)
{
	bool isPlaying = true;
	(*channel)->isPlaying(&isPlaying);
	if (!isPlaying)
	{
		system->playSound(sound, 0, false, channel);
	}

	(*channel)->setPaused(false);
	isPlaying = (*channel)->getCurrentSound(&sound);
}

void PurahEngine::SoundManager::Update()
{
	system->update();
}

void PurahEngine::SoundManager::Set3DListenerAttributes(FMOD_VECTOR pos, FMOD_VECTOR forward, FMOD_VECTOR up)
{
	system->set3DListenerAttributes(0, &pos, 0, &forward, &up);
}

FMOD::System* PurahEngine::SoundManager::GetSystem() const
{
	return system;
}

PurahEngine::SoundManager::SoundManager()
	: system(nullptr), bgmChannel(nullptr), effectChannel(nullptr), effectChannelGroup(nullptr)
{

}

PurahEngine::SoundManager::~SoundManager()
{

}

PurahEngine::SoundManager& PurahEngine::SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}