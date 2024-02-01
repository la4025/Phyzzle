#include "TestMovement.h"
#include "TimeController.h"
#include "SoundManager.h"

#define M_PI       3.14159265358979323846


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

	/// 게임패드 관련
	const int DEADZONE = inputManager.Getinstance().deadZone;
	int thumbLX = inputManager.Getinstance().GetState().Gamepad.sThumbLX;
	int thumbLY = inputManager.Getinstance().GetState().Gamepad.sThumbLY;


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
		trans->Rotate(trans->front, -angle);
	}

	if (inputManager.IsKeyPressed('W') == true ||
		thumbLY > 0)
	{
		movement += Eigen::Vector3f(0.0f, 0.0f, 0.1f);
	}
	if (inputManager.IsKeyPressed('S') == true ||
		thumbLY < 0)
	{
		movement += Eigen::Vector3f(0.0f, 0.0f, -0.1f);
	}
	if (inputManager.IsKeyPressed('A') == true ||
		thumbLX < 0)
	{
		movement += Eigen::Vector3f(-0.1f, 0.0f, 0.0f);
	}
	if (inputManager.IsKeyPressed('D') == true ||
		thumbLX > 0)
	{
		movement += Eigen::Vector3f(0.1f, 0.0f, 0.0f);
	}
	if (inputManager.IsKeyPressed(VK_SPACE) == true ||
		inputManager.Getinstance().GetState().Gamepad.wButtons == XINPUT_GAMEPAD_A)
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
		rotationValue += PurahEngine::TimeController::GetInstance().GetDeltaTime("Init") * 10;
		GetGameObject()->GetComponent<PurahEngine::Transform>()->SetLocalRotation(q);
	}

	const auto localPos = trans->GetLocalPosition();
	const auto worldPos = trans->GetWorldPosition();

	// Transform 에게 줘라
	//rigid->SetPosition(localPos + movement);

	trans->SetLocalPosition(localPos + movement);


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

	//{
	//	Eigen::Affine3f transformation = Eigen::Affine3f::Identity();
	//	transformation.translation() << 1.0f, 2.0f, 3.0f; // 이동
	//	transformation.rotate(Eigen::AngleAxisf(45.0f * M_PI / 180.0f, Eigen::Vector3f::UnitY())); // 회전
	//	transformation.scale(2.0f); // 크기 조절

	//	// 아핀 변환 행렬 decompose
	//	Eigen::Vector3f translation = transformation.translation();
	//	Eigen::Quaternionf rotation_quaternion2(transformation.linear()); // 회전 매트릭스를 Quaternionf로 변환
	//	Eigen::Vector3f scaling;
	//	scaling[0] = transformation.linear().col(0).norm(); // x 축의 크기
	//	scaling[1] = transformation.linear().col(1).norm(); // y 축의 크기
	//	scaling[2] = transformation.linear().col(2).norm(); // z 축의 크기

	//	transformation.scale(1.0f);
	//	scaling[0] = transformation.linear().col(0).norm(); // x 축의 크기
	//	scaling[1] = transformation.linear().col(1).norm(); // y 축의 크기
	//	scaling[2] = transformation.linear().col(2).norm(); // z 축의 크기
	//	Eigen::Quaternionf rotation_quaternion(transformation.linear()); // 회전 매트릭스를 Quaternionf로 변환
 //		transformation.scale(1.0f);
	//}
}
