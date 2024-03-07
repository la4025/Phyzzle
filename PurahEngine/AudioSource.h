#pragma once
#include "PurahEngineAPI.h"
#include "Component.h"

#include "Transform.h"

namespace PurahEngine
{
	class AudioSource : public Component
	{
	public:
		void Awake();
		void Initialize();
		void Update();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		Transform* soundTransform;
	};
}

