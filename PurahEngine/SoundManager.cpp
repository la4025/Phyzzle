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

void PurahEngine::SoundManager::CreateSound(SoundType soundType, std::wstring name, FMOD::Sound** sound)
{
	switch (soundType)
	{
		case SoundType::BGM:
		{
			CreateBGMSound(name, sound);
			break;
		}
		case SoundType::EFFECT:
		{
			CreateSfxSound(name, sound);
			break;
		}
	}
}

void PurahEngine::SoundManager::ReleaseSound(AudioSource* audioSource)
{

}

void PurahEngine::SoundManager::CreateBGMSound(std::wstring name, FMOD::Sound** sound)
{
	FMOD_RESULT result;
	std::wstring filePath = L"../Sound/BGM/Test/" + name;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::CreateSfxSound(std::wstring name, FMOD::Sound** sound)
{
	FMOD_RESULT result;
	std::wstring filePath = L"../Sound/SFX/Test/" + name;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_3D_LINEARSQUAREROLLOFF, 0, sound);
	assert(result == FMOD_OK);
}

void PurahEngine::SoundManager::PlayAudio(SoundType soundType, FMOD::Sound* sound, FMOD::Channel** channel)
{
	switch (soundType)
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