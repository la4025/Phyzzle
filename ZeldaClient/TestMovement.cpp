#include "TestMovement.h"
#include "TimeController.h"
#include "SoundManager.h"

PurahEngine::TestMovement::TestMovement()
{

}

PurahEngine::TestMovement::~TestMovement()
{

}

int j = 0;
void PurahEngine::TestMovement::Update()
{
	j++;
	const float angle = 10.0f;

	auto& inputManager = PurahEngine::InputManager::Getinstance();

	const auto rigid = GetGameObject()->GetComponent<RigidBody>();
	const auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();
	auto movement = Eigen::Vector3f{ 0.f, 0.f, 0.f };

	if (inputManager.IsKeyPressed('Q') == true)
	{
		const auto axis = trans->up;
		trans->Rotate(axis, angle);
	}
	if (inputManager.IsKeyPressed('E') == true)
	{
		const auto axis = trans->up;
		trans->Rotate(axis, -angle);
	}

	if (inputManager.IsKeyPressed('W') == true)
	{
		movement += Eigen::Vector3f(0.0f, 0.0f, 0.1f);
	}
	if (inputManager.IsKeyPressed('S') == true)
	{
		movement += Eigen::Vector3f(0.0f, 0.0f, -0.1f);
	}
	if (inputManager.IsKeyPressed('A') == true)
	{
		movement += Eigen::Vector3f(-0.1f, 0.0f, 0.0f);
	}
	if (inputManager.IsKeyPressed('D') == true)
	{
		movement += Eigen::Vector3f(0.1f, 0.0f, 0.0f);
	}
	if (inputManager.IsKeyPressed(VK_SPACE) == true)
	{
		movement += Eigen::Vector3f(0.0f, 0.1f, 0.0f);
	}

	if (inputManager.IsKeyPressed('Z') == true)
	{
		trans->SetLocalScale(trans->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (inputManager.IsKeyPressed('C') == true)
	{
		trans->SetLocalScale(trans->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
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

	const auto localPos = trans->GetLocalPosition();
	
	rigid->SetPosition(localPos + movement);

	// trans->SetLocalPosition(localPos + movement);


	/// Sound Test-----------------------------------------------------------
	if (inputManager.IsKeyPressed('8') == true)
	{
		PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test001");
	}

	if (inputManager.IsKeyPressed('9') == true)
	{
		PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test002");
	}

	if (inputManager.IsKeyPressed('0') == true)
	{
		PurahEngine::SoundManager::GetInstance().Play(L"BGM_Test003");
	}

	OutputDebugStringW((std::to_wstring(j) + L" ").c_str());
}

int i = 0;

void PurahEngine::TestMovement::FixedUpdate()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
		i++;
		OutputDebugStringW((std::to_wstring(i) + L"\n").c_str());
}
