#include "AudioListener.h"

#include "GameObject.h"
#include "SoundManager.h"

PurahEngine::AudioListener::AudioListener()
	: listenerTransform(nullptr)
{

}

PurahEngine::AudioListener::~AudioListener()
{

}

void PurahEngine::AudioListener::Initialize()
{
	listenerTransform = GetGameObject()->GetTransform();

}

void PurahEngine::AudioListener::Update()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();

	Eigen::Vector3f listenerPosition = listenerTransform->GetWorldPosition();
	FMOD_VECTOR pos = { listenerPosition.x(), listenerPosition.y(), listenerPosition.z() };
	FMOD_VECTOR forward = { listenerTransform->GetFront().x(), listenerTransform->GetFront().y(), listenerTransform->GetFront().z() };
	FMOD_VECTOR up = { listenerTransform->GetUp().x(), listenerTransform->GetUp().y(), listenerTransform->GetUp().z() };
	soundManager.SetListenerPosition(pos, forward, up);
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
