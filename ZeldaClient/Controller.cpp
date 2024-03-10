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
				onVibration = true;
			}
			//if (gamePad->IsKeyDown(ePad::ePAD_B))
			//{
			//	onVibration = false;
			//	if (tween)
			//	{
			//		Tween::Release(tween);
			//	}
			//}

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

			velo += velocity;

			// Apply velocity to the character's rigidbody
			rigidbody->SetLinearVelocity(velocity);
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
