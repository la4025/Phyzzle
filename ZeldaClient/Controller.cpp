#include "Controller.h"


namespace PurahEngine
{
	Controller::~Controller()
	= default;

	void Controller::Awake()
	{
		rigidbody = gameObject->GetComponent<RigidBody>();
		transform = gameObject->GetComponent<Transform>();
		speed = 5.f;
		drag = 0.7f;
	}

	void Controller::Start()
	{
		startPosition = transform->GetWorldPosition();
		startRotation = transform->GetWorldRotation();
		startLinearVelocity = rigidbody->GetLinearVelocity();
		startAngularVelocity = rigidbody->GetAngularVelocity();
	}

	void Controller::Update()
	{
		InputManager& instance = InputManager::Getinstance();
		const bool w = instance.IsKeyPressed('W');
		const bool s = instance.IsKeyPressed('S');
		const bool a = instance.IsKeyPressed('A');
		const bool d = instance.IsKeyPressed('D');

		const bool q = instance.IsKeyPressed('Q');
		const bool e = instance.IsKeyPressed('E');

		const bool r = instance.IsKeyDown('R');

		const bool space = instance.IsKeyPressed(VK_SPACE);

		Eigen::Vector3f velo = rigidbody->GetLinearVelocity();

		if (w || s || a || d || q || e || r)
		{
			Eigen::Vector3f direction{ 0.f, 0.f, 0.f };

			if (w)
			{
				direction += transform->front;
			}

			if (s)
			{
				direction -= transform->front;
			}

			if (a)
			{
				direction -= transform->right;
			}

			if (d)
			{
				direction += transform->right;
			}

			if (q)
			{
				direction -= transform->up;
			}

			if (e)
			{
				direction += transform->up;
			}

			if (r)
			{
				transform->SetWorldPosition(startPosition);
				transform->SetWorldRotation(startRotation);
				rigidbody->SetLinearVelocity(startLinearVelocity);
				rigidbody->SetAngularVelocity(startAngularVelocity);
			}

			velo += direction * speed;
		}

		rigidbody->SetLinearVelocity(velo * drag);
	}

	void Controller::FixedUpdate()
	{

	}
}
