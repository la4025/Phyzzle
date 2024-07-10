#include "PauseGame.h"
#include "TimeController.h"
#include "GameObject.h"
#include "UnifiedInputManager.h"
#include "Player.h"

using namespace PurahEngine;

namespace Phyzzle
{
	void PauseGame::Awake()
	{
		isPause = false;
	}

	void PauseGame::Start()
	{

	}

	void PauseGame::Update()
	{
		for (auto& [targetGameObject, state] : onOffEventList)
		{
			bool targetState = targetGameObject->GetEnable();
			if (UnifiedInputManager::Getinstance().GetKeyDown(eUnfInput::UI_MENU))
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

	void PauseGame::LateUpdate()
	{
		if (isPause)
		{
			TimeController::GetInstance().SetTimeScale(0.0f);

			if (!previousIsPause) // 이전 상태가 false일 때만 호출
			{
				//player->SetStopUpdate(true);
			}
		}
		else
		{
			TimeController::GetInstance().SetTimeScale(1.0f);

			if (previousIsPause) // 이전 상태가 true일 때만 호출
			{
				player->SetStopUpdate(false);
			}
		}

		previousIsPause = isPause; // 현재 상태를 저장
	}

	void PauseGame::PreSerialize(json& jsonData) const
	{

	}

	void PauseGame::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void PauseGame::PostSerialize(json& jsonData) const
	{

	}

	void PauseGame::PostDeserialize(const json& jsonData)
	{
		onOffEventList.clear();
		for (int i = 0; i < jsonData[std::string("__ID__onOffEventList")].size(); i++)
		{
			GameObject* targetObject = static_cast<GameObject*>(PurahEngine::FileManager::GetInstance().GetAddress(jsonData[std::string("__ID__onOffEventList")][i][0]));
			bool state = jsonData[std::string("__ID__onOffEventList")][i][1];

			onOffEventList.push_back({ targetObject, state });
		}
		POSTDESERIALIZE_PTR(player);
	}
}