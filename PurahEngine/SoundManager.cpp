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

	result = system->set3DSettings(1.0, 1.0, 1.0);

	system->createChannelGroup("effectChannelGroup", &effectChannelGroup);

	LoadSound(L"BGM_Test001", L"../Sound/BGM/Test/BGMTest001.mp3", SoundType::BGM);
	LoadSound(L"BGM_Test002", L"../Sound/BGM/Test/BGMTest002.mp3", SoundType::BGM);
	LoadSound(L"BGM_Test003", L"../Sound/BGM/Test/BGMTest003.mp3", SoundType::BGM);
	LoadSound(L"Effect_Test001", L"../Sound/Effect/Test/EffectTest001.mp3", SoundType::EFFECT);
	LoadSound(L"Effect_Test002", L"../Sound/Effect/Test/EffectTest002.mp3", SoundType::EFFECT);
	LoadSound(L"Effect_Test003", L"../Sound/Effect/Test/EffectTest003.mp3", SoundType::EFFECT);
}

void PurahEngine::SoundManager::LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type)
{
	FMOD::Sound* sound;
	FMOD_RESULT result;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	switch (type)
	{
		case SoundType::BGM:
		{
			result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, &sound);
			
			assert(result == FMOD_OK);

			bgmSounds[soundName] = sound;

			break;
		}

		case SoundType::EFFECT:
		{
			result = system->createSound(str.c_str(), FMOD_3D, 0, &sound);
			assert(result == FMOD_OK);

			result = sound->set3DMinMaxDistance(0.5f, 100.0f);

			effectSounds[soundName] = sound;

			break;
		}

	}
}

void PurahEngine::SoundManager::Play(const std::wstring& soundName)
{
	FMOD::Sound* sound;
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

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

	iter = effectSounds.find(soundName);
	if (iter != effectSounds.end())
	{
		sound = iter->second;
		system->playSound(sound, 0, false, &effectChannel);
	}

}

void PurahEngine::SoundManager::Update()
{
	FMOD_VECTOR lastPos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	vel.x = (listenerPosition.x - lastPos.x);
	vel.y = (listenerPosition.y - lastPos.y);
	vel.z = (listenerPosition.z - lastPos.z);

	//FMOD_VECTOR forward = PurahEngine::Transform::GetFront();
	system->set3DListenerAttributes(0, &listenerPosition, &vel, &forward, &up);
	system->update();
	lastPos = listenerPosition;
}

void PurahEngine::SoundManager::SetListenerPosition(FMOD_VECTOR lPosition, FMOD_VECTOR lFront, FMOD_VECTOR lUp)
{
	listenerPosition = lPosition;
	forward = lFront;
	up = lUp;
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