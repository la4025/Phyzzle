#pragma once
#include <Eigen/Dense>

namespace physx
{
	class PxController;
}

namespace ZonaiPhysics
{
	// template <typename _Base, typename _Ty>
	class CharacterController
	{
	public:
		CharacterController();

	public:
		/// <summary>
		/// Setter
		/// </summary>
		void SetCollisonData();
		void SetPosition(const Eigen::Vector3f& _position);
		void SetFootPosition(const Eigen::Vector3f& _position);
		void SetStepOffset(const float _offset);
		void SetContactOffset(const float _offset);
		void SetUpDirection(const Eigen::Vector3f& _direction);
		void SetSlope(const float _slope);

		/// <summary>
		/// Getter
		/// </summary>
		Eigen::Vector3f GetPosition();
		Eigen::Vector3f GetFootPosition();
		float GetStepOffset();
		float GetContactOffset();
		Eigen::Vector3f GetUpDirection();
		float GetSlope();

		/// <summary>
		/// Function
		/// </summary>
		void Move(Eigen::Vector3f _vec, float _dis, float _dt);

	public:
		physx::PxController* controller;
	};
}
