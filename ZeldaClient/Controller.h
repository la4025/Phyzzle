#pragma once
#include "PurahEngine.h"

// #define PROPERTY(type, value) type #value; \ 

namespace ZonaiPhysics
{
	class Collider;
}

namespace PurahEngine
{
	class ITween;
}

namespace Phyzzle
{
	class Controller : public PurahEngine::Component
	{
	public:
		enum class State
		{
			IDLE,
			RUNNING,
		};

		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

		void OnCollisionEnter(const ZonaiPhysics::ZnCollision&, const PurahEngine::Collider*) override;

	private:
		void GamePadInput();
		void Move();
		void RotateCamera();

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

		PurahEngine::ITween* tween;

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

	private:
		/// 사라질 변수들
		Eigen::Vector3f startPosition;
		Eigen::Vector3f startLinearVelocity;
		Eigen::Vector3f startAngularVelocity;
		Eigen::Quaternionf startRotation;
	};
}

