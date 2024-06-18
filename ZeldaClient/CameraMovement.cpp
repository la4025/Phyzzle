#include "CameraMovement.h"
#include <iostream>

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
	
	// rotation
	if (inputManager.IsKeyPressed(eKey::eKEY_UP))
	{
		trans->Rotate(trans->GetWorldRotation() * Eigen::Vector3f::UnitX(), -0.5f);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_DOWN))
	{
		trans->Rotate(trans->GetWorldRotation() * Eigen::Vector3f::UnitX(), 0.5f);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_LEFT))
	{
		trans->Rotate(Eigen::Vector3f::UnitY(), -0.5f);
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_RIGHT))
	{
		trans->Rotate(Eigen::Vector3f::UnitY(), 0.5f);
	}

	
	// position
	if (inputManager.IsKeyPressed(eKey::eKEY_W))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetFront() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_A))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetRight() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_S))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetFront() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_D))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetRight() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_Q))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() - (trans->GetUp() * 0.1f));
	}
	if (inputManager.IsKeyPressed(eKey::eKEY_E))
	{
		trans->SetLocalPosition(trans->GetLocalPosition() + (trans->GetUp() * 0.1f));
	}
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
