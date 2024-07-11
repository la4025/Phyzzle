#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class JumpCheck : public PurahEngine::Component
	{
	public:
		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;

		void PreDeserialize(const json& jsonData) override;

		void PostSerialize(json& jsonData) const override;

		void PostDeserialize(const json& jsonData) override;
	};
}
