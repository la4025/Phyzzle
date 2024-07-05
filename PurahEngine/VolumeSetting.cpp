#include "VolumeSetting.h"
#include "GameObject.h"
#include "Button.h"
#include "UnifiedInputManager.h"
void PurahEngine::VolumeSetting::Awake()
{
	button = GetGameObject()->GetComponent<Button>();
}

void PurahEngine::VolumeSetting::Start()
{

}

void PurahEngine::VolumeSetting::Update()
{
	double soundVolume = 0.0f;
	bool isSelect = button->IsSelected();

	if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_Ok) && isSelect)
	{
		if (volumeType == VolumeType::UP)
		{
			if (soundType == SoundType::BGM)
			{
				soundVolume = SoundManager::GetInstance().GetBGMVolume();
				soundVolume += 0.1f;
				//soundVolume += 1.0f;
				//soundVolume /= 10;
				SoundManager::GetInstance().SetBGMVolume(soundVolume);
			}
			else
			{
				soundVolume = SoundManager::GetInstance().GetSFXVolume();
				soundVolume += 0.1f;
				//soundVolume += 1.0f;
				//soundVolume /= 10;
				SoundManager::GetInstance().SetSFXVolume(soundVolume);
			}
		}
		else if (volumeType == VolumeType::DOWN)
		{
			if (soundType == SoundType::BGM)
			{
				soundVolume = SoundManager::GetInstance().GetBGMVolume();
				soundVolume -= 0.1f;
				SoundManager::GetInstance().SetBGMVolume(soundVolume);
			}
			else
			{
				soundVolume = SoundManager::GetInstance().GetSFXVolume();
				soundVolume -= 0.1f;
				//soundVolume -= 1.0f;
				//soundVolume /= 10;
				SoundManager::GetInstance().SetSFXVolume(soundVolume);
			}
		}
	}
}

void PurahEngine::VolumeSetting::LateUpdate()
{

}

void PurahEngine::VolumeSetting::PreSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeSetting::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
	soundType = jsonData["soundType"];
	volumeType = jsonData["volumeType"];
}

void PurahEngine::VolumeSetting::PostSerialize(json& jsonData) const
{

}

void PurahEngine::VolumeSetting::PostDeserialize(const json& jsonData)
{

}

/// PreDeserialize 고치고, Unity Component 만들것.