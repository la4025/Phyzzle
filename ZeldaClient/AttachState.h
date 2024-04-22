#pragma once
#include "IState.h"
#include "Coroutine.h"

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
		float distance = -0.1f;
		Eigen::Vector3f playerVelocity = Eigen::Vector3f::Zero();
		Eigen::Vector3f targetVelocity = Eigen::Vector3f::Zero();
		PurahEngine::RigidBody* selectBody = nullptr;

	private:
		void PlayerMove() const;
		void CameraAround() const;
		void StateCancel() const;

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to);
		void LookToLocalDirection(const Eigen::Vector3f& _to);

		bool Search() const;
		bool Select();
		
		void ApplyPlayerVelocity() const;
		void ApplyObjectVelocity() const;

		void ObjectTranslate(const Eigen::Vector3f& _direction, float power);
		void ObjectRotate();

		Coroutine<int> Test();


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

