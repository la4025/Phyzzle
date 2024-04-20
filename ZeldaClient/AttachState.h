#pragma once
#include "IState.h"

// ReSharper disable once IdentifierTypo
namespace Phyzzle
{
	class AttachState final : public IState
	{
	public:
		AttachState() = delete;
		explicit AttachState(Player* _player)
			: IState(_player)
		{}
		~AttachState() override;

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

		void Click_DUp() override;
		void Click_DDown() override;
		void Click_DLeft() override;
		void Click_DRight() override;

		void Click_LB() override;
		void Click_RB() override;

	private:
		bool selected = false;
		bool rotateMode = false;
		float lerpTime = 0.1f;

	private:
		bool hasGravity = false;
		float mass = -0.1f;
		PurahEngine::RigidBody* selectBody = nullptr;
		Eigen::Vector3f targetLocalPosition = Eigen::Vector3f::Zero();
		Eigen::Quaternionf targetLocalRotation = Eigen::Quaternionf::Identity();

	private:
		void PlayerMove() const;
		void CameraAround() const;
		void StateCancel() const;

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to);
		void LookToLocalDirection(const Eigen::Vector3f& _to);

		bool Search() const;
		bool Select();
		
		void ObjectToTargetPosition() const;

		void ObjectTranslate(const Eigen::Vector3f& _direction, float power);
		void ObjectRotate();
		
		void Attach() const;

		void Set();
		void Reset();

	private:
		void SearchDebugDraw(bool _value);

		Eigen::Vector3f debugVector0 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector1 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector2 = Eigen::Vector3f::Zero();
	};
}

