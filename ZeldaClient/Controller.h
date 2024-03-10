#pragma once
#include "PurahEngine.h"

namespace PurahEngine
{
	class Controller : public Component
	{
	public:
		~Controller() override;

		void Awake() override;
		void Start() override;

		void Update() override;

	private:
		void Move();
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
		float speed;
		float drag;

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

