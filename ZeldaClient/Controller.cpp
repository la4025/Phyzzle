#include "Controller.h"

#include "TimeController.h"
#include "Tween.h"
#include <algorithm>

namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		moveSpeed = 3.f;
		sensitivity = 45.f;
	}

	void Controller::Start()
	{
		GamePadManager::AddGamePad(0);

		gamePad = GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(5000);

		startPosition = modelCore->GetWorldPosition();
		startRotation = modelCore->GetWorldRotation();

		// playerRigidbody = gameObject->GetComponent<RigidBody>();
		startLinearVelocity = playerRigidbody->GetLinearVelocity();
 		startAngularVelocity = playerRigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		GamePadInput();
		RotateCamera();
		// UpdateCamera();
		Move();
	}

	void Controller::GamePadInput()
	{
		LstickX = 0.f, LstickY = 0.f;
		RstickX = 0.f, RstickY = 0.f;
		LTrigger = 0.f, RTrigger = 0.f;

		if (gamePad->IsConnected())
		{

			gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);

			LTrigger = gamePad->GetTriggerRatio(ePadTrigger::ePAD_TRIGGER_L);
			RTrigger = gamePad->GetTriggerRatio(ePadTrigger::ePAD_TRIGGER_R);

			if (gamePad->IsKeyDown(ePad::ePAD_A))
			{
				gamePad->VibrateRatio(LTrigger, RTrigger, 3.f);
			}
			if (gamePad->IsKeyDown(ePad::ePAD_B))
			{
				gamePad->VibrateRatio(0.f, 0.f);
			}
		}
	}

	void Controller::Move()
	{
		// 카메라의 전방 벡터를 계산
		Eigen::Vector3f cameraFront = cameraArm->GetFront();
		const Eigen::Vector3f forward = Eigen::Vector3f(cameraFront.x(), 0.f, cameraFront.z()).normalized();
		const Eigen::Vector3f right = cameraArm->GetRight();

		Eigen::Vector3f movementDirection = forward * LstickY + right * LstickX;

		movementDirection.y() = 0.f;

		// 속도 벡터를 계산
		Eigen::Vector3f movement = movementDirection * moveSpeed;

		Eigen::Vector3f velocity = playerRigidbody->GetLinearVelocity();

		// y축 운동을 방해하지 않는 걸로 중력은 적용되도록함.
		/// 단, 이런 방식이면 키 입력이 없다면 누군가랑 부딪쳐도 가만히 있을 듯
		velocity.x() = movement.x();
		velocity.z() = movement.z();

		// 속력을 적용시킴
		playerRigidbody->SetLinearVelocity(velocity);

		//if (LstickY == 0.f && LstickX == 0.f)
		//{
		//	return;
		//}

		const Eigen::Quaternionf parentWorld = gameObject->GetTransform()->GetWorldRotation();
		const Eigen::Vector3f localForward = parentWorld.conjugate() * movementDirection.normalized();

		// Calculate the rotation quaternion to align the current forward direction with the desired forward direction
		const Eigen::Quaternionf targetRotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), localForward);

		// Set the rotation of the transform to the target rotation
		modelCore->SetLocalRotation(targetRotation);
	}

	void Controller::RotateCamera()
	{
		{
			TimeController& time = TimeController::GetInstance();

			const float deltaTime = time.GetDeltaTime("Simulate");

			// 스틱 기울기에 따라 회전 각도를 계산
			const float yawAngle = RstickX * sensitivity * deltaTime;
			const float pitchAngle = RstickY * sensitivity * deltaTime;

			// 월드 up 기준으로 카메라를 회전
			cameraArm->Rotate(Eigen::Vector3f(0.f, 1.f, 0.f), yawAngle);

			// 카메라 Right 벡터를 기준으로 회전하기 위해서 카메라의 월드 right를 구함.
			const Eigen::Vector3f cameraRight = cameraArm->GetWorldRotation() * -Eigen::Vector3f::UnitX();

			// 카메라의 right 기준으로 카메라를 회전
			cameraArm->Rotate(cameraRight, pitchAngle);
		}
	}

	void Controller::HandsUp()
	{

	}

	void Controller::PreSerialize(json& jsonData) const
	{

	}

	void Controller::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(detect);
		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(sensitivity);
	}

	void Controller::PostSerialize(json& jsonData) const
	{

	}

	void Controller::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_PTR(playerRigidbody);
		POSTDESERIALIZE_PTR(modelCore);
		POSTDESERIALIZE_PTR(cameraArm);
		POSTDESERIALIZE_PTR(cameraCore);
	}
}
