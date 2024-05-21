#pragma once
#include "PurahEngine.h"


namespace  Phyzzle
{
	class PlayerController : public PurahEngine::Component
	{
	public:
		~PlayerController() override;

	public:
		void Move(const Eigen::Vector3f& _dir, float _speed);
		void LookTo(const Eigen::Vector3f& _worldDirection);
		void LookAt(const Eigen::Vector3f& _worldPosition);
		void Jump();

		void SetSlope(float _angle);

		bool CanJump();
		bool IsGround();
		bool IsWall();
		bool IsObstacle();

	private:
		PurahEngine::RigidBody* body;
		PurahEngine::Transform* model;

	private:
		bool jumping = false;
		float slope;
	};
}

