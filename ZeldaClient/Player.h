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
	public:
		~Player() override;

		enum State
		{
			DEFAULT		= 0,	// 기본 상태
			HOLD		= 1,	// 물건을 든 상태
			ATTATCH		= 2,	// 물건을 부착하려는 상태
			REWIND		= 3,	// 물건을 되돌리려는 상태
			LOCK		= 4
		};

	private:
		struct PlayerData
		{
			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;
			Holder* holder;

			State state;

			float xAngle = 0.f;
			const float limitHighAngle = 80.f;
			const float limitLowAngle = -70.f;

			float moveSpeed = 10.f;
			float holdSpeed = 5.f;
			float sensitivity = 90.f;

			float jumpPower = 10.f;
			bool jumping = false;
		};

		struct StickData
		{
			float X;
			float Y;
			float Size;

			const StickData& operator=(const StickData& _other)
			{
				if (this != &_other)
				{
					X = _other.X;
					Y = _other.Y;
					Size = _other.Size;
				}

				return *this;
			}
			const StickData& operator=(StickData&& _other) noexcept
			{
				if (this != &_other)
				{
					X = std::move(_other.X);
					Y = std::move(_other.Y);
					Size = std::move(_other.Size);
				}

				return *this;
			}
		};

		struct PlayerInput
		{
			PurahEngine::GamePad::State buttonA;
			PurahEngine::GamePad::State buttonB;
			PurahEngine::GamePad::State buttonX;
			PurahEngine::GamePad::State buttonY;

			PurahEngine::GamePad::State buttonUP;
			PurahEngine::GamePad::State buttonDOWN;
			PurahEngine::GamePad::State buttonLEFT;
			PurahEngine::GamePad::State buttonRIGHT;

			PurahEngine::GamePad::State buttonLB;
			PurahEngine::GamePad::State buttonRB;

			StickData Lstick;
			StickData Rstick;
			
			float LTrigger;
			float RTrigger;

			const PlayerInput& operator=(const PlayerInput& _other)
			{
				if (this != &_other)
				{
					buttonA = _other.buttonA;
					buttonB = _other.buttonB;
					buttonX = _other.buttonX;
					buttonY = _other.buttonY;

					buttonUP = _other.buttonUP;
					buttonDOWN = _other.buttonDOWN;
					buttonLEFT = _other.buttonLEFT;
					buttonRIGHT = _other.buttonRIGHT;

					buttonLB = _other.buttonLB;
					buttonRB = _other.buttonRB;
				
					Lstick = _other.Lstick;
					Rstick = _other.Rstick;
					
					LTrigger = _other.LTrigger;
					RTrigger = _other.RTrigger;
				}

				return *this;
			}

			const PlayerInput& operator=(PlayerInput&& _other) noexcept
			{
				if (this != &_other)
				{
					buttonA = std::move(_other.buttonA);
					buttonB = std::move(_other.buttonB);
					buttonX = std::move(_other.buttonX);
					buttonY = std::move(_other.buttonY);

					buttonUP = std::move(_other.buttonUP);
					buttonDOWN = std::move(_other.buttonDOWN);
					buttonLEFT = std::move(_other.buttonLEFT);
					buttonRIGHT = std::move(_other.buttonRIGHT);

					buttonLB = std::move(_other.buttonLB);
					buttonRB = std::move(_other.buttonRB);

					Lstick = std::move(_other.Lstick);
					Rstick = std::move(_other.Rstick);

					LTrigger = std::move(_other.LTrigger);
					RTrigger = std::move(_other.RTrigger);
				}

				return *this;
			}
		};

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
		friend class IState;
		friend class DefaultState;
		friend class AttatchState;
		friend class RewindState;
		friend class DefaultState;

		std::unordered_map<State, IState*> stateSystem;
		
		State prevState = DEFAULT;
		State currState = DEFAULT;

		PurahEngine::GamePad* gamePad;
		PlayerInput currInput;
		PlayerInput prevInput;

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

