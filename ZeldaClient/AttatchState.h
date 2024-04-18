#pragma once
#include "IState.h"

// ReSharper disable once IdentifierTypo
namespace Phyzzle
{
	class AttatchState final : public IState
	{
	public:
		AttatchState() = delete;
		explicit AttatchState(Player* _player)
			: IState(_player)
		{}
		~AttatchState() override;

	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
		void Stick_L() override;
		void Stick_R() override;

		void Click_A() override;
		void Click_B() override;
		void Click_X() override;
		void Click_Y() override;

		void Click_LB() override;
		void Click_RB() override;

	private:
		bool selected = false;
		float lerpTime = 0.1f;

	private:
		bool hasGravity = false;
		float mass = -0.1f;
		PurahEngine::RigidBody* selectBody = nullptr;
		Eigen::Vector3f targetLocalPosition = Eigen::Vector3f::Zero();
		Eigen::Quaternionf targetLocalRotation = Eigen::Quaternionf::Identity();

		void PlayerMove() const;
		void CameraAround() const;
		void StateCancel() const;

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to) const;
		void LookToLocalDirection(const Eigen::Vector3f& _to) const;

		bool Search() const;
		bool Select();
		void ObjectToTargetPosition() const;

		void Set();
		void Reset();

		static void SearchDebugDraw(bool _value);
	};
}

