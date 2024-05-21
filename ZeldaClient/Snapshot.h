#pragma once
#include <chrono>
#include <Eigen/Dense>

namespace Phyzzle
{
	struct Snapshot
	{
		Snapshot() : step(),
			position(Eigen::Vector3f::Zero()), rotation(Eigen::Quaternionf::Identity()),
			linearVelocity(Eigen::Vector3f::Zero()), angularVelocity(Eigen::Quaternionf::Identity())
		{}
		Snapshot(const Snapshot& _other) : step(_other.step),
			position(_other.position), rotation(_other.rotation),
			linearVelocity(_other.linearVelocity), angularVelocity(_other.linearVelocity)
		{}
		Snapshot(Snapshot&& _other) noexcept : step(_other.step),
			position(_other.position), rotation(_other.rotation),
			linearVelocity(_other.linearVelocity), angularVelocity(_other.linearVelocity)
		{}
		Snapshot& operator=(const Snapshot& _other)
		{
			if (this != &_other)
			{
				step = _other.step;
				position = _other.position;
				rotation = _other.rotation;
				linearVelocity = _other.linearVelocity;
				angularVelocity = _other.angularVelocity;
			}

			return *this;
		}
		Snapshot& operator=(Snapshot&& _other) noexcept
		{
			if (this != &_other)
			{
				step = _other.step;
				position = _other.position;
				rotation = _other.rotation;
				linearVelocity = _other.linearVelocity;
				angularVelocity = _other.angularVelocity;
			}

			return *this;
		}

		float step;
		Eigen::Vector3f position;
		Eigen::Quaternionf rotation;
		Eigen::Vector3f linearVelocity;
		Eigen::Vector3f angularVelocity;
	};
}

