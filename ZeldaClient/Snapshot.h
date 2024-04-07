#pragma once
#include <chrono>
#include <Eigen/Dense>

namespace Phyzzle
{
	struct Snapshot
	{
		std::chrono::system_clock::time_point time;
		std::chrono::duration<float> duration;
		Eigen::Vector3f position;
		Eigen::Quaternionf rotation;
		Eigen::Vector3f linearVelocity;
		Eigen::Vector3f angularVelocity;

		bool operator<(const Snapshot& _rhs)
		{
			return time < _rhs.time;
		}
	};
}

