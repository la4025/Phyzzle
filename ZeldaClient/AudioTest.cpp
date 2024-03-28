#include "AudioTest.h"

PurahEngine::AudioTest::AudioTest()
{

}

PurahEngine::AudioTest::~AudioTest()
{

}

void PurahEngine::AudioTest::OnDataLoadComplete()
{

}

void PurahEngine::AudioTest::Update()
{

}

void PurahEngine::AudioTest::OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*)
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	soundManager.PlayEffect(audioSource->GetSoundName(), audioSource);
}

void PurahEngine::AudioTest::PreSerialize(json& jsonData) const
{

}

void PurahEngine::AudioTest::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::AudioTest::PostSerialize(json& jsonData) const
{

}

void PurahEngine::AudioTest::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_PTR(audioSource);
}
