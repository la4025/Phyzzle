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
			levelTriggers[i]->SetTarget(targetObject);
		}

		for (int i = 0; i < deathTriggers.size(); i++)
		{
			deathTriggers[i]->RegisterTargetSystem(this);
			deathTriggers[i]->SetMode(RespawnTrigger::Mode::deathTrigger);
			deathTriggers[i]->SetTarget(targetObject);
		}
	}

	void RespawnSystem::OnDeath()
	{
		switch (mode)
		{
			case Mode::Nearest:
			{
				Eigen::Vector3f targetPos = targetObject->GetTransform()->GetWorldPosition();

				float minDist = - 1.0f;
				int minIdx = -1;

				for (int i = 0; i < respawnPoints.size(); i++)
				{
					Eigen::Vector3f respawnPoint = respawnPoints[i]->GetWorldPosition();
					float dist = (targetPos - respawnPoint).norm();

					if (minIdx == -1 || dist < minDist)
					{
						minDist = dist;
						minIdx = i;
					}
				}

				if (keepRotation)
				{
					targetObject->GetTransform()->SetWorldPosition(respawnPoints[minIdx]->GetWorldPosition());
				}
				else
				{
					targetObject->GetTransform()->SetWorldMatrix(respawnPoints[minIdx]->GetWorldMatrix());
				}

				break;
			}
			case Mode::MaxLevel:
			{
				if (keepRotation)
				{
					targetObject->GetTransform()->SetWorldPosition(respawnPoints[maxLevel]->GetWorldPosition());
				}
				else
				{
					targetObject->GetTransform()->SetWorldMatrix(respawnPoints[maxLevel]->GetWorldMatrix());
				}
				break;
			}
			case Mode::LastLevel:
			{
				if (keepRotation)
				{
					targetObject->GetTransform()->SetWorldPosition(respawnPoints[lastLevel]->GetWorldPosition());
				}
				else
				{
					targetObject->GetTransform()->SetWorldMatrix(respawnPoints[lastLevel]->GetWorldMatrix());
				}
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
		PREDESERIALIZE_VALUE(mode);
		PREDESERIALIZE_VALUE(keepRotation);
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