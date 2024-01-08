#include "TestMovement.h"
#include "TimeController.h"
PurahEngine::TestMovement::TestMovement()
{

}

PurahEngine::TestMovement::~TestMovement()
{

}

void PurahEngine::TestMovement::Update()
{
	float angle = 10.0f;

	auto& inputManager = PurahEngine::InputManager::Getinstance();

	if (inputManager.IsKeyPressed('Q') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->Rotate(GetGameObject()->GetComponent<PurahEngine::Transform>()->up, angle);
	}

	if (inputManager.IsKeyPressed('E') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->Rotate(Eigen::Vector3f::UnitY(), -angle);
	}

	if (inputManager.IsKeyPressed('W') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed('S') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed('A') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed('D') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalPosition(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}

	if (inputManager.IsKeyPressed('Z') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalScale(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (inputManager.IsKeyPressed('C') == true)
	{
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalScale(GetGameObject()->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	static float rotationValue = 0.0f;




	if (inputManager.IsKeyPressed('B') == true)
	{
		float roll = 0.0f;    // X 축 회전
		float pitch = rotationValue;   // Y 축 회전
		float yaw = 0.0f;     // Z 축 회전

		// 오일러 각도를 쿼터니언으로 변환
		Eigen::Quaternionf q;
		q = Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitZ())
			* Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
			* Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitX()) * Eigen::Quaternionf::Identity();
		float getDelta = PurahEngine::TimeController::GetInstance().GetDeltaTime("Init");
		rotationValue += PurahEngine::TimeController::GetInstance().GetDeltaTime("Init") * 90;
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetWorldRotation(q);
	}
}
