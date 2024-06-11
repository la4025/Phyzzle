#include "Burnable.h"

namespace Phyzzle
{
	void Burnable::Awake()
	{
		burnTimeElapsed = 0.0f;
		waitDestroy = false;
		destroyElapsed = 0.0f;

		if (effectObject != nullptr)
		{
			effectObject->SetEnable(false);
		}
	}

	void Burnable::Update()
	{
		float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		if (fireList.size() > 0ull)
		{
			burnTimeElapsed += deltaTime;
		}
		else
		{
			burnTimeElapsed = 0.0f;
		}

		if (fireList.size() > 0ull && burnTimeElapsed >= burnTime)
		{
			burnableObject->SetEnable(false);
			effectObject->GetComponent<PurahEngine::ParticleSystem>()->StopGenerate();
			waitDestroy = true;
		}

		if (waitDestroy)
		{
			destroyElapsed += deltaTime;

			if (destoryDelay <= destroyElapsed)
			{
				gameObject->Destroy();
			}
		}
	}

	void Burnable::OnTriggerEnter(const PurahEngine::Collider* other)
	{
		if (other->GetGameObject()->tag.IsContain(L"Fire"))
		{
			fireList.push_back(other);
		}

		if (effectObject != nullptr)
		{
			if (fireList.size() > 0u)
			{
				effectObject->SetEnable(true);
				effectObject->GetComponent<PurahEngine::ParticleSystem>()->Reset();
			}
			else
			{
				effectObject->SetEnable(false);
			}
		}
	}

	void Burnable::OnTriggerExit(const PurahEngine::Collider* other)
	{
		if (other->GetGameObject()->tag.IsContain(L"Fire"))
		{
			auto iter = std::find(fireList.begin(), fireList.end(), other);
			if (iter != fireList.end())
			{
				fireList.erase(iter);
			}
		}

		if (effectObject != nullptr)
		{
			if (fireList.size() > 0u)
			{
				effectObject->SetEnable(true);
				effectObject->GetComponent<PurahEngine::ParticleSystem>()->Reset();
			}
			else
			{
				effectObject->SetEnable(false);
			}
		}
	}

	void Burnable::PreSerialize(json& jsonData) const
	{
	}

	void Burnable::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(burnTime);
		PREDESERIALIZE_VALUE(destoryDelay);
	}

	void Burnable::PostSerialize(json& jsonData) const
	{
	}

	void Burnable::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(burnableObject);
		POSTDESERIALIZE_PTR(effectObject);
	}
}