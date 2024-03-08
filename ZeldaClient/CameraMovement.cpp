#include "CameraMovement.h"

PurahEngine::CameraMovement::CameraMovement()
{

}

PurahEngine::CameraMovement::~CameraMovement()
{

}

void PurahEngine::CameraMovement::Update()
{
	auto& inputManager = PurahEngine::InputManager::Getinstance();
	auto trans = GetGameObject()->GetComponent<PurahEngine::Transform>();

	if (inputManager.IsKeyPressed(eKey::eKEY_UP))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_DOWN) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_LEFT) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_RIGHT) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_Q) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, -0.1f));
	}	
	if (inputManager.IsKeyPressed(eKey::eKEY_P) == true)
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.0f, 0.1f));
	}
	//if(inputManager.IsKeyPressed(VK_WHEEL))
}

void PurahEngine::CameraMovement::PreSerialize(json& jsonData) const
{

}

void PurahEngine::CameraMovement::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE();
}

void PurahEngine::CameraMovement::PostSerialize(json& jsonData) const
{

}

void PurahEngine::CameraMovement::PostDeserialize(const json& jsonData)
{

}
