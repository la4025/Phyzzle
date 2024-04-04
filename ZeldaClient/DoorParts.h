#pragma once

#include "ElectricalDevice.h"

#include <vector>

namespace Phyzzle
{
	class DoorParts : public ElectricalDevice
	{
	private:
		enum class State : int
		{
			Open,
			Close
		};

	public:
		void Awake() override;

		void Update() override;

		void PowerOn() override;
		void PowerOff() override;

	private:
		void MoveToTarget(const Eigen::Vector3f& target);

	private:
		Eigen::Vector3f movePos;
		float speed;

		Eigen::Vector3f targetPos;
		Eigen::Vector3f originPos;
		int powerCounter;
		State state;

		bool done;

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
	};
}