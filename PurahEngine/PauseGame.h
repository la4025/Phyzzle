#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

namespace PurahEngine
{


	class PURAHENGINE_API PauseGame : public Component
	{

	public:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;

	private:
		bool isPause;

		std::vector<std::pair<GameObject*, bool>> onOffEventList;

	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}

