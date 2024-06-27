#include "PauseGame.h"
#include "TimeController.h"
#include "GameObject.h"
#include "UnifiedInputManager.h"

void PurahEngine::PauseGame::Awake()
{
	isPause = false;
}

void PurahEngine::PauseGame::Start()
{

}

void PurahEngine::PauseGame::Update()
{
	for (auto& [targetGameObject, state] : onOffEventList)
	{
		bool targetState = targetGameObject->GetEnable();
		if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_Cancel))
		{
			targetGameObject->SetEnable(!targetState);
		}

		if (targetState == true)
		{
			isPause = true;
		}
		else
		{
			isPause = false;
		}

	}
}

void PurahEngine::PauseGame::LateUpdate()
{
	if (isPause)
	{
		TimeController::GetInstance().SetTimeScale(0.0f);
	}
	else
	{
		TimeController::GetInstance().SetTimeScale(1.0f);

	}
}

void PurahEngine::PauseGame::PreSerialize(json& jsonData) const
{

}

void PurahEngine::PauseGame::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::PauseGame::PostSerialize(json& jsonData) const
{

}

void PurahEngine::PauseGame::PostDeserialize(const json& jsonData)
{
	onOffEventList.clear();
	for (int i = 0; i < jsonData[std::string("__ID__onOffEventList")].size(); i++)
	{
		GameObject* targetObject = static_cast<GameObject*>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__onOffEventList")][i][0]));
		bool state = jsonData[std::string("__ID__onOffEventList")][i][1];

		onOffEventList.push_back({ targetObject, state });
	}
}
