#include "Controller.h"

#include "TimeController.h"
#include <iostream>

namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		rigidbody = playerBody->GetComponent<RigidBody>();
		transform = gameObject->GetTransform();
		speed = 5.f;
 		drag = 0.7f;
	}

	void Controller::Start()
	{
		GamePadManager::AddGamePad(0);

		gamePad = GamePadManager::GetGamePad(0);
		gamePad->SetDeadZone(1000);

		startPosition = transform->GetWorldPosition();
		startRotation = transform->GetWorldRotation();
		startLinearVelocity = rigidbody->GetLinearVelocity();
 		startAngularVelocity = rigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		Move();
	}

	void Controller::Move()
	{
		InputManager& instance = InputManager::Getinstance();
		TimeController& time = TimeController::GetInstance();


		float LstickX = 0.f , LstickY = 0.f;
		float RstickX = 0.f , RstickY = 0.f;

		if (gamePad->IsConnected())
		{
			std::cout << LstickX << "    " << LstickY << std::endl;
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_L, LstickX, LstickY);
			gamePad->GetStickRatio(ePadStick::ePAD_STICK_R, RstickX, RstickY);
			if (gamePad->IsKeyDown(ePad::ePAD_A))
			{
			}
				gamePad->VibrateRatio(0.f, 0.f);
		
		}

		// playerBody 이동
		const bool w = instance.IsKeyPressed(eKey::eKEY_W);
		const bool s = instance.IsKeyPressed(eKey::eKEY_S);
		const bool a = instance.IsKeyPressed(eKey::eKEY_A);
		const bool d = instance.IsKeyPressed(eKey::eKEY_D);

		const bool q = instance.IsKeyPressed(eKey::eKEY_Q);
		const bool e = instance.IsKeyPressed(eKey::eKEY_E);

		const bool up = instance.IsKeyPressed(eKey::eKEY_UP);
		const bool down = instance.IsKeyPressed(eKey::eKEY_DOWN);
		const bool left = instance.IsKeyPressed(eKey::eKEY_LEFT);
		const bool right = instance.IsKeyPressed(eKey::eKEY_RIGHT);

		const bool r = instance.IsKeyDown(eKey::eKEY_R);

		const bool space = instance.IsKeyPressed(eKey::eKEY_SPACE);

		Eigen::Vector3f velo = rigidbody->GetLinearVelocity();

		// 속도로 이동하고 drag로 멈추려고 하는데
		// drag가 크면 떨어지는 속도도 느려짐
		// drag로 멈추는건 수정해야할듯
		//if (w || s || a || d || q || e || r)
		//{
			Eigen::Vector3f direction{ 0.f, 0.f, 0.f };
			const auto world = transform->GetWorldRotation();
			direction += world * transform->front * LstickY;
			direction += world * transform->right * LstickX;

			//if (w)
			//{
			//	direction += world * transform->front;
			//}

			//if (s)
			//{
			//	direction -= world * transform->front;
			//}

			//if (a)
			//{
			//	direction -= world * transform->right;
			//}

			//if (d)
			//{
			//	direction += world * transform->right;
			//}


			//if (q)
			//{
			//	direction -= world * transform->up;
			//}

			//if (e)
			//{
			//	direction += world * transform->up;
			//}

			if (r)
			{
				transform->SetWorldPosition(startPosition);
				transform->SetWorldRotation(startRotation);
				rigidbody->SetLinearVelocity(startLinearVelocity);
				rigidbody->SetAngularVelocity(startAngularVelocity);
			}

			// direction.normalize();
			velo += direction * speed;

			const auto playerT = playerBody->GetTransform();
			// playerT->SetWorldRotation({ 0.f, direction.x(), direction.y(), direction.z() });
			transform->SetWorldRotation(world);
		//}

		rigidbody->SetLinearVelocity(velo * drag);

		if (up | down | left | right)
		{
			const float dt = time.GetDeltaTime("Simulate");
			const float angle = 45.f * dt;
			const auto world = transform->GetWorldRotation();

			if (up)
			{
				transform->Rotate(world * transform->right, -angle);
			}

			if (down)
			{
				transform->Rotate(world * transform->right, angle);
			}

			if (left)
			{
				transform->Rotate(transform->up, -angle);
			}

			if (right)
			{
				transform->Rotate(transform->up, angle);
			}
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
