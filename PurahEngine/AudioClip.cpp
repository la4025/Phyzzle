#include "AudioClip.h"


PurahEngine::AudioClip::AudioClip()
	: mSound(nullptr), mChannel(nullptr)
{

}

PurahEngine::AudioClip::~AudioClip()
{
	mSound->release();
	mSound = nullptr;
}

void PurahEngine::AudioClip::CreateSound()
{
	soundManager.LoadSound(newSound, &mSound);
}

void PurahEngine::AudioClip::Play()
{
	soundManager.Play(newSound, mSound, &mChannel);
}

void PurahEngine::AudioClip::Set3DAttributes(FMOD_VECTOR pos)
{
	mChannel->set3DAttributes(&pos, 0);
}
