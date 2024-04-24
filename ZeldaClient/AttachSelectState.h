#pragma once
#include "IState.h"
#include "Coroutine.h"

namespace Phyzzle
{
	class AttachSelectState final : public IState
	{
		enum Mode
		{
			SELECT	= 0,
			HOLD	= 1,
			ROTATE	= 2,
		};

	public:
		AttachSelectState() = delete;
		explicit AttachSelectState(Player* _player)
			: IState(_player)
		{}
		~AttachSelectState() override;

	private:
		void StateEnter() override;
		void StateExit() override;
		void StateStay() override;

	private:
#pragma region Input
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

		void Pressing_DUp() override;
		void Pressing_DDown() override;
		void Pressing_DLeft() override;
		void Pressing_DRight() override;

		void Click_LB() override;

		void Pressing_RB() override;
#pragma endregion Input

	private:
		Mode mode = SELECT;

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
		void PlayerMove(float _speed) const;
		void CameraAround() const;
		void StateCancel() const;

		void Jump() const;

		void LookToWorldDirection(const Eigen::Vector3f& _to);
		void LookToLocalDirection(const Eigen::Vector3f& _to);

		bool Search() const;
		bool Select();
		
		void ApplyObjectVelocity() const;
		void ResetObjectVelocity();

		void ObjectTranslate(const Eigen::Vector3f& _direction, float power);
		void ObjectRotate();

		void Attach() const;

		void Set();
		void Reset();

	private:
#if _DEBUG
		void SearchDebugDraw(bool _value);

		Eigen::Vector3f debugVector0 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector1 = Eigen::Vector3f::Zero();
		Eigen::Vector3f debugVector2 = Eigen::Vector3f::Zero();
#endif _DEBUG
	};
}

