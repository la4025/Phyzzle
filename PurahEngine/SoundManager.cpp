#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "SoundManager.h"
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
	result = system->init(512, FMOD_INIT_NORMAL, nullptr);

	assert(result == FMOD_OK);

	system->createChannelGroup("effectChannelGroup", &effectChannelGroup);

	LoadSound(L"BGM_Test001", L"../Sound/BGM/Test/BGMTest001.mp3", SoundType::BGM);
	LoadSound(L"BGM_Test002", L"../Sound/BGM/Test/BGMTest002.mp3", SoundType::BGM);
	LoadSound(L"BGM_Test003", L"../Sound/BGM/Test/BGMTest003.mp3", SoundType::BGM);
}

void PurahEngine::SoundManager::LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type)
{
	FMOD::Sound* sound;
	FMOD_RESULT result;

	switch (type)
	{
		case SoundType::BGM:
		{
			// wstring을 string으로 변환하는 방법
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			std::string str = converter.to_bytes(filePath);

			result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, &sound);

			assert(result == FMOD_OK);

			bgmSounds[soundName] = sound;

			break;
		}

		case SoundType::EFFECT:
		{

			break;
		}

	}
}

void PurahEngine::SoundManager::Play(const std::wstring& soundName)
{
	FMOD::Sound* sound;

	auto iter = bgmSounds.find(soundName);
	if (iter != bgmSounds.end())
	{
		sound = iter->second;

		bool isPlaying;
		bgmChannel->isPlaying(&isPlaying);
		
		if (isPlaying)
		{
			bgmChannel->stop();
		}

		system->playSound(sound, 0, false, &bgmChannel);
	}
}

void PurahEngine::SoundManager::Update()
{
	system->update();
}

PurahEngine::SoundManager::SoundManager()
	: system(nullptr), bgmChannel(nullptr), effectChannelGroup(nullptr)
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