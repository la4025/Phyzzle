#include "ShockDetector.h"

#include "ElectricalDevice.h"

namespace Phyzzle
{
	void ShockDetector::Awake()
	{
		isPowerOn = false;
		shock = 0.0f;
	}

	void ShockDetector::Update()
	{
		if (activationShock < shock)
		{
			shock = 0.0f;

			TargetPowerOn();
			TargetPowerOff();
		}
	}

	void ShockDetector::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* other)
	{
		float currentShock = collision.impulses.norm();
		if (shock < currentShock) shock = currentShock;
	}

	void ShockDetector::TargetPowerOn()
	{
		if (isPowerOn == true)
		{
			return;
		}

		isPowerOn = true;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOn();
		}
	}

	void ShockDetector::TargetPowerOff()
	{
		if (onlyPowerOn)
		{
			return;
		}

		if (isPowerOn == false)
		{
			return;
		}

		isPowerOn = false;

		for (int i = 0; i < targetDeviceList.size(); i++)
		{
			targetDeviceList[i]->PowerOff();
		}
	}

	void ShockDetector::PreSerialize(json& jsonData) const
	{
	}
	void ShockDetector::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(onlyPowerOn);
		PREDESERIALIZE_VALUE(activationShock);
	}
	void ShockDetector::PostSerialize(json& jsonData) const
	{
	}
	void ShockDetector::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_VECTOR_PTR(targetDeviceList);
	}
}