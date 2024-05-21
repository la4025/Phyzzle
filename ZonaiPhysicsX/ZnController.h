#pragma once
#include <Eigen/Dense>

namespace ZonaiPhysics
{
	class ZnController
	{
	public:
		ZnController(physx::PxController* _controller);


		void Move(Eigen::Vector3f _vec, float _dis, float _dt);

	private:
		physx::PxController* controller;
	};
}
