#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"
#include "PurahSound.h"

#include "Transform.h"
#include "SoundManager.h"

namespace PurahEngine
{
	class SoundManager;

	class AudioClip : public Component
	{
	public:
		AudioClip();
		~AudioClip();

		void Initialize();
		void OnDataLoadComplete();
		void Set3DAttributes(const Eigen::Vector3f pos);

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		PurahEngine::PurahSound newSound;

	private:
		SoundManager& soundManager = SoundManager::GetInstance();

	};
}