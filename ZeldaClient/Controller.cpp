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
		gamePad->SetDeadZone(2000);

		startPosition = myTransform->GetWorldPosition();
		startRotation = myTransform->GetWorldRotation();

		playerRigidbody = gameObject->GetComponent<RigidBody>();
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
		if (gamePad->IsConnected())
		{
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);

			float LTrigger = 0.f, RTrigger = 0.f;
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
		TimeController& time = TimeController::GetInstance();

		if (gamePad->IsConnected())
		{
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);

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

		{
			// Calculate movement direction based on the camera's forward vector
			Eigen::Vector3f cameraForward = myTransform->GetWorldRotation() * myTransform->front;
			Eigen::Vector3f cameraRight = myTransform->GetWorldRotation() * myTransform->right;
			Eigen::Vector3f movementDirection = cameraForward * LstickY + cameraRight * LstickX;

			movementDirection.y() = 0.f;

			// Calculate velocity based on the movement direction and speed
			Eigen::Vector3f movement = movementDirection * moveSpeed;

			Eigen::Vector3f velocity = playerRigidbody->GetLinearVelocity();

			velocity.x() = movement.x();
			velocity.z() = movement.z();

			// Apply velocity to the character's playerRigidbody
			playerRigidbody->SetLinearVelocity(velocity);
		}
	}

	void Controller::RotateCamera()
	{
		{
			TimeController& time = TimeController::GetInstance();

			float RstickX = 0.f, RstickY = 0.f;
			if (gamePad->IsConnected())
			{
				gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);
			}

			float deltaTime = time.GetDeltaTime("Simulate");

			// Calculate rotation angles based on input from the right stick
			float yawAngle = RstickX * sensitivity * deltaTime;
			float pitchAngle = RstickY * sensitivity * deltaTime;

			// Rotate the character (and thus the camera) around the world's up vector (yaw)
			myTransform->Rotate(myTransform->up, yawAngle);

			// Calculate the camera's forward vector after the yaw rotation
			Eigen::Vector3f cameraForward = myTransform->GetWorldRotation() * myTransform->front;

			// Calculate the camera's right vector based on the updated forward vector and the world's up vector
			Eigen::Vector3f cameraRight = cameraForward.cross(myTransform->up).normalized();

			// Rotate the camera around its right vector (pitch)
			myTransform->Rotate(cameraRight, pitchAngle);
		}

		//TimeController& time = TimeController::GetInstance();

		//float RstickX = 0.f, RstickY = 0.f;
		//if (gamePad->IsConnected()) {
		//	gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);
		//}

		//float deltaTime = time.GetDeltaTime("Simulate");

		//// Calculate rotation angles based on input from the right stick
		//float yawAngle = RstickX * sensitivity * deltaTime;
		//float pitchAngle = RstickY * sensitivity * deltaTime;

		//// Convert the current rotation quaternion to Euler angles
		//Eigen::Vector3f currentEulerAngles = myTransform->GetLocalRotation().toRotationMatrix().eulerAngles(0, 1, 2);

		//// Extract the pitch angle
		//float currentPitch = currentEulerAngles.x();

		//// Calculate the new pitch angle and constrain it within -80 and 80 degrees
		//float newPitch = currentPitch + pitchAngle;
		//pitchAngle = max(min(newPitch, 80.f * (std::numbers::pi / 180.f)), -80.f * (std::numbers::pi / 180.f)) - currentPitch;

		//// Rotate the character (and thus the camera) around the world's up vector (yaw)
		//myTransform->Rotate(myTransform->up, yawAngle);

		//// Calculate the camera's forward vector after the yaw rotation
		//Eigen::Vector3f cameraForward = myTransform->GetWorldRotation() * myTransform->front;

		//// Calculate the camera's right vector based on the updated forward vector and the world's up vector
		//Eigen::Vector3f cameraRight = cameraForward.cross(myTransform->up).normalized();

		//// Rotate the camera around its right vector (pitch)
		//myTransform->Rotate(myTransform->right, pitchAngle);
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
		POSTDESERIALIZE_PTR(playerGameObject);
		POSTDESERIALIZE_PTR(playerRigidbody);
		POSTDESERIALIZE_PTR(myTransform);
	}

	void Controller::SetPlayer(GameObject* _player)
	{
		assert(_player != nullptr);

		playerGameObject = _player;
	}
}
