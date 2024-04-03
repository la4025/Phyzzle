#pragma once
#include "PurahEngine.h"

// #define PROPERTY(type, value) type #value; \ 

namespace ZonaiPhysics
{
	class Collider;
}

namespace Phyzzle
{
	class Player final : public PurahEngine::Component
	{
	public:
		enum class State
		{
			DEFAULT,	// 기본 상태
			HOLD,		// 물건을 든 상태
			ATTATCH,	// 물건을 부착하려는 상태
			REWIND,		// 물건을 되돌리려는 상태
		}; 

		~Player() override;

		void Awake() override;
		void Start() override;

		void Update() override;

		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

	private:
		void GamePadInput();
		void Move();
		void RotateCamera();

		void Jump();

		void HandsUp();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		// void SetPlayer(GameObject*);

	private:
		// __declspec(property(get = , put = ));
		bool detect;
		float moveSpeed;
		float sensitivity;

		float jumpPower;

		bool jumping = false;

	private:
		PurahEngine::RigidBody* playerRigidbody;
		PurahEngine::Transform* modelCore;
		PurahEngine::Transform* cameraArm;
		PurahEngine::Transform* cameraCore;
		PurahEngine::Animator* animator;

	private:
		Eigen::Vector3f movement = Eigen::Vector3f::Zero();

	private:
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
		float vibrationL = 0.f;
		float vibrationR = 0.f;

		float xAngle = 0.f;
		const float limitHighAngle = 80.f;
		const float limitLowAngle = -70.f;


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

