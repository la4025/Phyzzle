#pragma once

#include "PurahEngineAPI.h"

#include "Component.h"

namespace PurahEngine
{
	class PURAHENGINE_API ButtonGroup : public Component
	{
	public:



	private:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}