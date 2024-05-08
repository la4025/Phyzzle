#include "AudioTest.h"


PurahEngine::AudioTest::AudioTest()
{

}

PurahEngine::AudioTest::~AudioTest()
{

}

void PurahEngine::AudioTest::Awake()
{
	auto& soundManager = PurahEngine::SoundManager::GetInstance();
	//soundManager.PlayBGM(audioSource->GetSoundName(), audioSource);
}

void PurahEngine::AudioTest::OnDataLoadComplete()
{

}

void PurahEngine::AudioTest::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();

	if (inputManager.IsKeyPressed(eKey::eKEY_M))
	{
		auto& soundManager = PurahEngine::SoundManager::GetInstance();
		soundManager.PlayEffect(audioSource->GetSoundName(), audioSource);
	}
}

void PurahEngine::AudioTest::OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const Collider*)
{
	
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
