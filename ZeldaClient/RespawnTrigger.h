#pragma once

#include "PurahEngine.h"

namespace Phyzzle
{
	class RespawnSystem;

	class RespawnTrigger final : public PurahEngine::Component
	{
	public:
		enum class Mode
		{
			levelTrigger,
			deathTrigger
		};

		void RegisterTargetSystem(RespawnSystem* target);
		void SetMode(Mode mode);
		void SetLevel(int level);
		void SetTargetTag(const std::wstring& tag);

		void OnTriggerEnter(const PurahEngine::Collider* other) override;

	private:
		RespawnSystem* targetSystem = nullptr;
		Mode mode;
		int level;
		std::wstring targetTag;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}