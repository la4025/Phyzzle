#include "AudioClip.h"


PurahEngine::AudioClip::AudioClip()
	: mSound(nullptr), mChannel(nullptr), mMinDistance(1.0f), mMaxDistance(1000.0f), isLoop(false)
{

}

PurahEngine::AudioClip::~AudioClip()
{
	mSound->release();
	mSound = nullptr;
}

void PurahEngine::AudioClip::Play()
{
	if (isLoop)
	{
		mSound->setMode(FMOD_LOOP_NORMAL);
	}

	//soundManager.PlayEffect();
}

void PurahEngine::AudioClip::Set3DAttributes()
{

}
