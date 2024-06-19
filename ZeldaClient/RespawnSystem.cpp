#include "RespawnSystem.h"

#include "RespawnTrigger.h"

namespace Phyzzle
{
	void RespawnSystem::Awake()
	{
		lastLevel = 0;
		maxLevel = 0;

		for (int i = 0; i < levelTriggers.size(); i++)
		{
			levelTriggers[i]->RegisterTargetSystem(this);
			levelTriggers[i]->SetMode(RespawnTrigger::Mode::levelTrigger);
			levelTriggers[i]->SetLevel(i);
			levelTriggers[i]->SetTargetTag(targetTag);
		}

		for (int i = 0; i < deathTriggers.size(); i++)
		{
			deathTriggers[i]->RegisterTargetSystem(this);
			deathTriggers[i]->SetMode(RespawnTrigger::Mode::deathTrigger);
			deathTriggers[i]->SetTargetTag(targetTag);
		}
	}

	void RespawnSystem::OnDeath()
	{
		switch (mode)
		{
			case Mode::Nearest:
			{
				Eigen::Vector3f targetPos = targetObject->GetTransform()->GetWorldPosition();

				float maxDist = - 1.0f;
				int maxIdx = -1;

				for (int i = 0; i < respawnPoints.size(); i++)
				{
					Eigen::Vector3f respawnPoint = respawnPoints[i]->GetWorldPosition();
					float dist = (targetPos - respawnPoint).size();

					if (maxDist < dist)
					{
						maxDist = dist;
						maxIdx = i;
					}
				}

				targetObject->GetTransform()->SetWorldPosition(respawnPoints[maxIdx]->GetWorldPosition());

				break;
			}
			case Mode::MaxLevel:
			{
				targetObject->GetTransform()->SetWorldPosition(respawnPoints[maxLevel]->GetWorldPosition());
				break;
			}
			case Mode::LastLevel:
			{
				targetObject->GetTransform()->SetWorldPosition(respawnPoints[lastLevel]->GetWorldPosition());
				break;
			}
			default:
			{
				assert(0);
				break;
			}
		}
	}

	void RespawnSystem::OnLevel(int level)
	{
		if (maxLevel < level)
		{
			maxLevel = level;
		}
		
		lastLevel = level;
	}

	void RespawnSystem::PreSerialize(json& jsonData) const
	{
	}

	void RespawnSystem::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_WSTRING(targetTag);
	}

	void RespawnSystem::PostSerialize(json& jsonData) const
	{
	}

	void RespawnSystem::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(targetObject);
		POSTDESERIALIZE_VECTOR_PTR(respawnPoints);
		POSTDESERIALIZE_VECTOR_PTR(levelTriggers);
		POSTDESERIALIZE_VECTOR_PTR(deathTriggers);
	}
}