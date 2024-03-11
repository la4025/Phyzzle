#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class ITween;

	class Controller : public Component
	{
	public:
		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

	private:
		void Move();
		void RotateCamera();
		void UpdateCamera();

		Eigen::Vector3f InterpolatePosition(const Eigen::Vector3f& positionAtNeg90, const Eigen::Vector3f& positionAt0, const Eigen::Vector3f& positionAt90, float t);
		Eigen::Vector3f Lerp(const Eigen::Vector3f& start, const Eigen::Vector3f& end, float t);

		void HandsUp();

	public:
		void PreSerialize(json& jsonData) const override;
		void PreDeserialize(const json& jsonData) override;
		void PostSerialize(json& jsonData) const override;
		void PostDeserialize(const json& jsonData) override;

	public:
		void SetPlayer(GameObject*);

	private:
		bool detect;
		float moveSpeed;
		float sensitivity;
		bool onVibration = false;
		float vibrationL = 0.f;
		float vibrationR = 0.f;
		ITween* tween;

		GameObject* playerBody;
		RigidBody* rigidbody;
		Transform* transform;

		GamePad* gamePad;

		/// 사라질 변수들
	private:
		Eigen::Vector3f startPosition;
		Eigen::Vector3f startLinearVelocity;
		Eigen::Vector3f startAngularVelocity;
		Eigen::Quaternionf startRotation;
	};
}

