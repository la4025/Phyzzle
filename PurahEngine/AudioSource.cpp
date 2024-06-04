#include "AudioSource.h"

#include "GameObject.h"

#include "InputManager.h"
#include "GamePadManager.h"

PurahEngine::AudioSource::AudioSource()
	: soundTransform(nullptr)
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
	// 파일 이름을 map에 넣고, value로 audioclip을 생성
	for (auto sound : soundFile)
	{
		std::wstring name = std::wstring(sound.name.begin(), sound.name.end());
		soundMap[name] = new AudioClip(name, sound.soundType, sound.minDistance, sound.maxDistance);
		soundMap[name]->CreateSound();
	}
}

void PurahEngine::AudioSource::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	Eigen::Vector3f soundPosition = soundTransform->GetWorldPosition();
	FMOD_VECTOR pos = { soundPosition.x(), soundPosition.y(), soundPosition.z() };
	
	for (auto iter = soundMap.begin(); iter != soundMap.end(); iter++)
	{
		iter->second->Set3DAttributes(pos);
	}
}


void PurahEngine::AudioSource::PlayAudio(std::wstring name)
{
	soundMap[name]->PlayAudio();
}


void PurahEngine::AudioSource::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();

	for (int i = 0; i < jsonData["soundFile"].size(); i++)
	{
		Sound sound;
		sound.name = jsonData["soundFile"][i]["fileName"];
		sound.soundType = jsonData["soundFile"][i]["soundType"];
		sound.minDistance = jsonData["soundFile"][i]["minDistance"];
		sound.maxDistance = jsonData["soundFile"][i]["maxDistance"];
		soundFile.push_back(sound);
	}
}

void PurahEngine::AudioSource::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(soundTransform);
}
