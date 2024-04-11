#pragma once
#include "PurahEngine.h"

namespace ZonaiPhysics
{
	struct ZnRaycastInfo;
	class Collider;
}

namespace Phyzzle
{
	class Holder;
	class IState;

	class Player final : public PurahEngine::Component
	{
	private:
		struct PlayerData
		{
			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;
			Holder* holder;

			float xAngle = 0.f;
			const float limitHighAngle = 80.f;
			const float limitLowAngle = -70.f;

			float moveSpeed = 10.f;
			float holdSpeed = 5.f;
			float sensitivity = 90.f;

			float jumpPower = 10.f;
			bool jumping = false;
		};

		struct PlayerInput
		{
			PurahEngine::GamePad* gamePad;
			bool onVibration = false;
			float LstickX;
			float LstickY;
			float LstickSize;
			float RstickSize;
			float RstickX;
			float RstickY;
			float LTrigger;
			float RTrigger;
		};

	public:
		enum class State
		{
			DEFAULT = 0,		// 기본 상태
			HOLD	= 1,		// 물건을 든 상태
			ATTATCH = 2,		// 물건을 부착하려는 상태
			REWIND	= 3,		// 물건을 되돌리려는 상태
		}; 

		~Player() override;

	public:
		void Start() override;
		void Update() override;
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

	private:
		void GamePadInput();

		void Jump();
		void JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);

		void Move(float _moveSpeed, bool _cameraLookAt);

		void CameraAround();
		void CameraForwardRaycast();


	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	private:
		void ChangeState(State);

	private:
		friend class DefaultState;
		friend class AttatchState;
		friend class RewindState;
		friend class DefaultState;
		std::unordered_map<State, IState*> stateSystem;
		State state = State::DEFAULT;
		PlayerInput pad;
		PlayerData data;

	private:
		/// 사라질 변수들
		Eigen::Vector3f startPosition;

		Eigen::Vector3f highPosition;
		Eigen::Vector3f lowPosition;

		Eigen::Vector3f differenceHigh;
		Eigen::Vector3f differenceLow;

		Eigen::Quaternionf startRotation;
	};
}

