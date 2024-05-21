#include "AudioClip.h"

PurahEngine::AudioClip::AudioClip()
{

}

PurahEngine::AudioClip::~AudioClip()
{
	newSound.sound->release();
	newSound.sound = nullptr;
}

void PurahEngine::AudioClip::Initialize()
{

}

void PurahEngine::AudioClip::OnDataLoadComplete()
{
	
}

void PurahEngine::AudioClip::Set3DAttributes(const Eigen::Vector3f pos)
{
	FMOD_VECTOR fmodPos(pos.x(), pos.y(), pos.z());
	FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);

	newSound.channel->set3DAttributes(&fmodPos, &fmodVel);
}

void PurahEngine::AudioClip::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioClip::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	PREDESERIALIZE_VALUE(newSound.type);
	PREDESERIALIZE_VALUE(newSound.minDistance);
	PREDESERIALIZE_VALUE(newSound.maxDistance);
	PREDESERIALIZE_WSTRING(newSound.soundName);

}

void PurahEngine::AudioClip::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioClip::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(newSound.transform);
}
