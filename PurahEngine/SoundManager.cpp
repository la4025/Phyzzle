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

	system->createChannelGroup("effectChannelGroup", &effectChannelGroup);
}

void PurahEngine::SoundManager::Finalize()
{
	system->release();
	system = nullptr;
}

void PurahEngine::SoundManager::LoadSound(const std::wstring& soundName, PurahEngine::Transform* transform, AudioSource* audioSource, SoundType type)
{
	switch (type)
	{
	case SoundType::BGM:
	{
		LoadBGMSound(soundName, audioSource);
		break;
	}
	case SoundType::EFFECT:
	{
		LoadEffectSound(soundName, transform, audioSource);
		break;
	}
	}
}

void PurahEngine::SoundManager::ReleaseSound(AudioSource* audioSource)
{
	soundMap.erase(audioSource);
}

void PurahEngine::SoundManager::LoadBGMSound(const std::wstring& soundName, AudioSource* audioSource)
{
	FMOD_RESULT result;

	PurahEngine::PurahSound newSound;
	std::wstring filePath = L"../Sound/BGM/Test/" + soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, &(newSound.sound));
	assert(result == FMOD_OK);

	newSound.soundName = soundName;
	newSound.isPlaying = false;
	newSound.channel = bgmChannel;
	newSound.type = SoundType::BGM;

	soundMap[audioSource] = newSound;
}

void PurahEngine::SoundManager::LoadEffectSound(const std::wstring& soundName, Transform* transform, AudioSource* audioSource)
{
	FMOD_RESULT result;

	PurahEngine::PurahSound newSound;
	std::wstring filePath = L"../Sound/Effect/Test/" + soundName;

	// wstring을 string으로 변환하는 방법
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string str = converter.to_bytes(filePath);

	result = system->createSound(str.c_str(), FMOD_3D_LINEARSQUAREROLLOFF, 0, &(newSound.sound));
	assert(result == FMOD_OK);

	newSound.soundName = soundName;
	newSound.transform = transform;
	newSound.isPlaying = false;
	newSound.channel = 0;
	newSound.type = SoundType::EFFECT;

	newSound.channel->setChannelGroup(effectChannelGroup);
	newSound.sound->set3DMinMaxDistance(1.0f, 100.0f);

	soundMap[audioSource] = newSound;
}

void PurahEngine::SoundManager::PlayBGM(const std::wstring& soundName, AudioSource* audioSource)
{
	auto purahSound = soundMap.at(audioSource);
	if (purahSound.soundName == soundName)
	{
		bool isBGMPlaying = true;
		purahSound.channel->isPlaying(&isBGMPlaying);

		if (isBGMPlaying)
		{
			purahSound.channel->stop();
		}

		system->playSound(purahSound.sound, 0, false, &purahSound.channel);
	}
}

void PurahEngine::SoundManager::PlayEffect(const std::wstring& soundName, AudioSource* audioSource)
{
	auto& purahSound = soundMap.at(audioSource);

	if (purahSound.soundName == soundName)
	{
		bool isPlaying = true;
		purahSound.channel->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			system->playSound(purahSound.sound, 0, false, &purahSound.channel);
		}

		purahSound.channel->setPaused(false);
		purahSound.isPlaying = purahSound.channel->getCurrentSound(&purahSound.sound);
	}
}

void PurahEngine::SoundManager::Update()
{
	system->update();
}

void PurahEngine::SoundManager::Set3DListenerAttributes(const PurahEngine::Transform* listenerTransform)
{
	Eigen::Vector3f pos = listenerTransform->GetWorldPosition();
	FMOD_VECTOR listenerPosition = { pos.x(), pos.y(), pos.z() };

	Eigen::Vector3f forward = listenerTransform->GetFront();
	FMOD_VECTOR listenerForward = { forward.x(), forward.y(), forward.z() };

	Eigen::Vector3f up = listenerTransform->GetUp();
	FMOD_VECTOR listenerUp = { up.x(), up.y(), up.z() };

	system->set3DListenerAttributes(0, &listenerPosition, 0, &listenerForward, &listenerUp);
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