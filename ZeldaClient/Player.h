#pragma once
#include <set>

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
			ATTACH_HOLD		= -1,	// 물건을 든 상태

			DEFAULT			= 0,	// 기본 상태
			ATTACH_SELECT	= 1,	// 물건을 부착하려는 상태
			REWIND_SELECT	= 2,	// 물건을 되돌리려는 상태
			LOCK_SELECT		= 3
		};

	private:
#pragma region Struct
		struct PlayerData
		{
			PurahEngine::RigidBody* playerRigidbody;
			PurahEngine::Transform* modelCore;
			PurahEngine::Transform* cameraArm;
			PurahEngine::Transform* cameraCore;
			PurahEngine::Animator* animator;

			State state = ATTACH_SELECT;

			Eigen::Vector3f		coreDefaultPosition;
			Eigen::Quaternionf	coreDefaultRotation;

			Eigen::Vector3f		armDefaultPosition;
			Eigen::Quaternionf	armDefaultRotation;

			float xAngle = 0.f;
			const float limitHighAngle = 80.f;
			const float limitLowAngle = -70.f;

			bool cameraUpdate = true;
			float lerpFactor = 0.2f;
			float acclerpFactor = 0.f;

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
			StickData Lstick;
			StickData Rstick;
			
			float LTrigger;
			float RTrigger;

			PlayerInput& operator=(const PlayerInput& _other)
			{
				if (this != &_other)
				{
			
					Lstick = _other.Lstick;
					Rstick = _other.Rstick;
					
					LTrigger = _other.LTrigger;
					RTrigger = _other.RTrigger;
				}

				return *this;
			}
			PlayerInput& operator=(PlayerInput&& _other) noexcept
			{
				if (this != &_other)
				{
					Lstick = std::move(_other.Lstick);
					Rstick = std::move(_other.Rstick);

					LTrigger = std::move(_other.LTrigger);
					RTrigger = std::move(_other.RTrigger);
				}

				return *this;
			}
		};
#pragma endregion Struct

	public:
#pragma region Event
		void Start() override;
		void Update() override;
		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
		void OnCollisionStay(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;
#pragma endregion Event

		void DebugDraw();

	private:
		void GamePadInput();

		void Jump();
		void JumpCheck(const ZonaiPhysics::ZnCollision& zn_collision, const PurahEngine::Collider* collider);

		void PlayerMove(float _moveSpeed);
		void LookInWorldDirection(const Eigen::Vector3f& _worldDirection) const;
		void LookInLocalDirection(const Eigen::Vector3f& _localDirection) const;

		void CameraCoreUpdate();
		void CameraReset();
		void CameraAround();

		bool CameraForwardRaycast(float _distance, PurahEngine::RigidBody** _outBody, float* _outDistance = nullptr, Eigen::Vector3f* _outHitPosition = nullptr);

	public:
#pragma region 직렬화
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;
#pragma endregion 직렬화

	private:
		void ChangeState(State);

	private:
		friend class IState;
		friend class DefaultState;
		friend class AttachSelectState;
		friend class AttachHoldState;
		friend class RewindState;
		friend class LockState;

		std::unordered_map<State, IState*> stateSystem;
		std::set<State> stateChange;
		
		State prevState = DEFAULT;
		State currState = DEFAULT;

		PurahEngine::GamePad* gamePad;
		PlayerInput currInput;
		PlayerInput prevInput;

		PlayerData data;
		std::function<Eigen::Vector3f(const Eigen::Vector3f, const Eigen::Vector3f, float)> lerp;
		std::function<Eigen::Quaternionf(const Eigen::Quaternionf, const Eigen::Quaternionf, float)> slerp;


	private:
		/// 사라질 변수들
		Eigen::Vector3f		highPosition;
		Eigen::Vector3f		lowPosition;

		Eigen::Vector3f		differenceHigh;
		Eigen::Vector3f		differenceLow;
	};

	inline Eigen::Vector3f MulMatrixVector(const Eigen::Matrix4f& _mat, const Eigen::Vector3f& _vec)
	{
		Eigen::Vector4f temp = _mat * Eigen::Vector4f(_vec.x(), _vec.y(), _vec.z(), 1.f);

		return { temp.x(), temp.y() , temp.z() };
	}
}

