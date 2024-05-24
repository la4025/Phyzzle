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
	for (auto name : fileName)
	{
		soundMap[name] = new AudioClip;
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


void PurahEngine::AudioSource::Play(std::wstring name)
{
	soundMap[name]->Play();
}


void PurahEngine::AudioSource::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_VECTOR(fileName);
}

void PurahEngine::AudioSource::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioSource::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(soundTransform);
}
