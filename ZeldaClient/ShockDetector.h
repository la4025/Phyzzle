#pragma once

#include "PurahEngine.h"

#include <vector>

namespace Phyzzle
{
	class ElectricalDevice;

	class ShockDetector final : public PurahEngine::Component
	{
	public:
		void Awake() override;
		void Update() override;

		// 충돌체가 충돌했을 때 호출
		virtual void OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other) override;

	private:
		void TargetPowerOn();
		void TargetPowerOff();

	private:
		bool onlyPowerOn;
		float activationShock;
		std::vector<ElectricalDevice*> targetDeviceList;

	private:
		bool isPowerOn;
		float shock;

	private:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}