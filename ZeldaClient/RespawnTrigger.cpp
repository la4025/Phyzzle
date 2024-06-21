#include "RespawnTrigger.h"

#include "RespawnSystem.h"

namespace Phyzzle
{
	void RespawnTrigger::RegisterTargetSystem(RespawnSystem* target)
	{
		targetSystem = target;
	}

	void RespawnTrigger::SetMode(Mode mode)
	{
		this->mode = mode;
	}

	void RespawnTrigger::SetLevel(int level)
	{
		this->level = level;
	}

	void RespawnTrigger::SetTarget(PurahEngine::GameObject* target)
	{
		targetObject = target;
	}

	void RespawnTrigger::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		if (targetSystem == nullptr)
		{
			return;
		}

		if (other->GetGameObject() == targetObject)
		{
			switch (mode)
			{
				case Mode::levelTrigger:
				{
					targetSystem->OnLevel(level);

					break;
				}
				case Mode::deathTrigger:
				{
					targetSystem->OnDeath();

					break;
				}
				default:
				{
					assert(0);
					break;
				}
			}
		}
	}

	void RespawnTrigger::PreSerialize(json& jsonData) const
	{
	}

	void RespawnTrigger::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
	}

	void RespawnTrigger::PostSerialize(json& jsonData) const
	{
	}

	void RespawnTrigger::PostDeserialize(const json& jsonData)
	{
	}
}