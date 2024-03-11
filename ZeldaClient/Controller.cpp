#include "Controller.h"

#include "TimeController.h"
#include "Tween.h"
#include <iostream>

namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		rigidbody = playerBody->GetComponent<RigidBody>();
		transform = gameObject->GetTransform();
		moveSpeed = 3.f;
		sensitivity = 45.f;
	}

	void Controller::Start()
	{
		GamePadManager::AddGamePad(0);

		gamePad = GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(2000);

		startPosition = transform->GetWorldPosition();
		startRotation = transform->GetWorldRotation();
		startLinearVelocity = rigidbody->GetLinearVelocity();
 		startAngularVelocity = rigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		RotateCamera();
		UpdateCamera();
		Move();
	}

	void Controller::Move()
	{
		TimeController& time = TimeController::GetInstance();

		float LstickX = 0.f , LstickY = 0.f;

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

			//if (onVibration)
			//{
			//	tween = Tween::DoTween<float>(
			//		vibrationL,					// start
			//		eCycleMode::LOOP,			// cycle
			//		1.f,						// end
			//		eEasing::eOutCirc,			// easing
			//		3.f,						// duration
			//		0.1f,						// delay
			//		[this]()					// callback
			//		{
			//			vibrationL = 0.f;
			//			vibrationR = 0.f;
			//		}
			//	);
			//	gamePad->VibrateRatio(vibrationL, vibrationR);
			//}
		}

		{
			// Calculate movement direction based on the camera's forward vector
			Eigen::Vector3f cameraForward = transform->GetWorldRotation() * transform->front;
			Eigen::Vector3f cameraRight = transform->GetWorldRotation() * transform->right;
			Eigen::Vector3f movementDirection = cameraForward * LstickY + cameraRight * LstickX;

			movementDirection.y() = 0.f;

			// Calculate velocity based on the movement direction and speed
			Eigen::Vector3f velocity = movementDirection * moveSpeed;

			Eigen::Vector3f velo = rigidbody->GetLinearVelocity();

			velo.x() = velocity.x();
			velo.z() = velocity.z();

			// Apply velocity to the character's rigidbody
			rigidbody->SetLinearVelocity(velo);
		}
	}

	void Controller::RotateCamera()
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
		transform->Rotate(transform->up, yawAngle);

		// Calculate the camera's forward vector after the yaw rotation
		Eigen::Vector3f cameraForward = transform->GetWorldRotation() * transform->front;

		// Calculate the camera's right vector based on the updated forward vector and the world's up vector
		Eigen::Vector3f cameraRight = cameraForward.cross(transform->up).normalized();

		// Rotate the camera around its right vector (pitch)
		transform->Rotate(cameraRight, pitchAngle);
	}

	void Controller::UpdateCamera()
	{
		// Get the current local rotation as a Quaternion
		Eigen::Quaternionf localRotation = transform->GetLocalRotation();

		// Convert the Quaternion to Euler angles
		Eigen::Vector3f localEulerAngles = localRotation.toRotationMatrix().eulerAngles(0, 1, 2);

		// Extract the pitch angle (rotation around the x-axis)
		float pitchAngle = localEulerAngles.x();

		// Clamp the pitch angle within the range [-90, 90]
		pitchAngle = std::clamp(pitchAngle, -90.0f, 90.0f);

		// Define desired camera positions at -90, 0, and 90 degrees pitch
		Eigen::Vector3f positionAt90(0, 10, 0);
		Eigen::Vector3f positionAt0(0, 2, -5);
		Eigen::Vector3f positionAtNeg90(0, 0, 0);

		// Determine the interpolation factor based on the current pitch angle
		float t;
		if (pitchAngle < 0) {
			t = 1 - (pitchAngle + 90) / 90; // Map pitch angle to [0, 1] range
		}
		else if (pitchAngle > 0) {
			t = pitchAngle / 90; // Map pitch angle to [0, 1] range
		}
		else {
			t = 0.5f; // Pitch angle is 0, use the middle position
		}

		// Interpolate between positions based on the pitch angle
		Eigen::Vector3f interpolatedPosition = InterpolatePosition(positionAtNeg90, positionAt0, positionAt90, t);

		// Set the camera's position to the interpolated position
		transform->SetLocalPosition(interpolatedPosition);
	}

	// Linear interpolation between three positions based on t
	Eigen::Vector3f Controller::InterpolatePosition(const Eigen::Vector3f& positionAtNeg90, const Eigen::Vector3f& positionAt0, const Eigen::Vector3f& positionAt90, float t)
	{
		// Ensure t is clamped within [0, 1]
		t = std::clamp(t, 0.0f, 1.0f);

		// Interpolate between positions based on t
		if (t <= 0.5f) {
			return Lerp(positionAtNeg90, positionAt0, t * 2);
		}
		else {
			return Lerp(positionAt0, positionAt90, (t - 0.5f) * 2);
		}
	}

	// Linear interpolation between two vectors based on t
	Eigen::Vector3f Controller::Lerp(const Eigen::Vector3f& start, const Eigen::Vector3f& end, float t)
	{
		return start + (end - start) * t;
	}

	void Controller::HandsUp()
	{

	}

	void Controller::PreSerialize(json& jsonData) const
	{
	}

	void Controller::PreDeserialize(const json& jsonData)
	{

	}

	void Controller::PostSerialize(json& jsonData) const
	{

	}

	void Controller::PostDeserialize(const json& jsonData)
	{

	}

	void Controller::SetPlayer(GameObject* _player)
	{
		assert(_player != nullptr);

		playerBody = _player;
	}
}
