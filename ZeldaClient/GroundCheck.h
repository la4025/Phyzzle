#pragma once
#include "PurahEngine.h"

namespace Phyzzle
{
	class GroundCheck : public PurahEngine::Component
	{
	private:
		int count = 0;
		bool isGrounded = false;

	public:
		bool IsGrounded();

		void OnTriggerEnter(const PurahEngine::Collider*) override;
		void OnTriggerStay(const PurahEngine::Collider*) override;
		void OnTriggerExit(const PurahEngine::Collider*) override;

		// Component을(를) 통해 상속됨
		void PreSerialize(json& jsonData) const override;

		void PreDeserialize(const json& jsonData) override;

		void PostSerialize(json& jsonData) const override;

		void PostDeserialize(const json& jsonData) override;
	};
}
