#pragma once
#include <Eigen/Dense>

namespace Phyzzle
{
	struct Snapshot
	{
		unsigned long long time;
		Eigen::Vector3f position;
		Eigen::Quaternionf rotation;
		Eigen::Vector3f linearVelocity;
		Eigen::Vector3f angularVelocity;
	};
}

