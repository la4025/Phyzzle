#include "AudioSource.h"

#include "GameObject.h"

#include "InputManager.h"
#include "GamePadManager.h"

PurahEngine::AudioSource::AudioSource()
	: soundTransform(nullptr), soundClip(nullptr)
{

}

PurahEngine::AudioSource::~AudioSource()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	soundManager.ReleaseSound(this);
}

void PurahEngine::AudioSource::Awake()
{

}

void PurahEngine::AudioSource::Initialize()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();

	soundTransform = GetGameObject()->GetTransform();
}

void PurahEngine::AudioSource::OnDataLoadComplete()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	soundManager.LoadSound(soundName, soundTransform, this, SoundType::EFFECT);
}

void PurahEngine::AudioSource::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	Eigen::Vector3f soundPosition = soundTransform->GetWorldPosition();
	soundClip->Set3DAttributes(soundPosition);
}

std::wstring PurahEngine::AudioSource::GetSoundName()
{
	return soundName;
}

void PurahEngine::AudioSource::SetClip(AudioClip* clip)
{
	soundClip = clip;
}

PurahEngine::AudioClip* PurahEngine::AudioSource::GetClip()
{
	return soundClip;
}

void PurahEngine::AudioSource::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_WSTRING(soundName);
}

void PurahEngine::AudioSource::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(soundTransform);
}
