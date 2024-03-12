#include "AudioListener.h"

PurahEngine::AudioListener::AudioListener()
	: listenerTransform(nullptr)
{

}

PurahEngine::AudioListener::~AudioListener()
{

}

void PurahEngine::AudioListener::OnDataLoadComplete()
{
	listenerTransform = GetGameObject()->GetTransform();

}

void PurahEngine::AudioListener::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();

	Eigen::Vector3f listenerPosition = listenerTransform->GetWorldPosition();
	FMOD_VECTOR pos = { listenerPosition.x(), listenerPosition.y(), listenerPosition.z() };
	soundManager.SetListenerPosition(pos);
}

void PurahEngine::AudioListener::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioListener::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::AudioListener::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioListener::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(listenerTransform);
}
