#include "KiHoon.h"
#include <cassert>

#include "PurahEngine.h"
#include <Eigen/Dense>
#include <DirectionalLight.h>

#include "TestMovement.h"
#include "CameraMovement.h"
#include "Controller.h"

void KiHoon::Run()
{
	using namespace PurahEngine;

	GameObject* light = SceneManager::GetInstance().CreateGameObject(L"Light");
	{
		MeshRenderer* renderer = light->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		const DirectionalLight* directional = light->AddComponent<DirectionalLight>();
		const auto trans = light->GetTransform();

		trans->SetLocalScale({ 1.f, 1.f, 2.f });
		trans->Rotate(Eigen::Vector3f{ 1.f, 0.f, 0.f }.normalized(), 45.f);
		trans->SetWorldPosition({0.f, 10.f, 0.f});

		const auto collider = light->AddComponent<BoxCollider>();
		collider->SetSize({ 0.5f, 0.5f, 1.f });
		collider->SetDynamic(true);
	}

	GameObject* light2 = SceneManager::GetInstance().CreateGameObject(L"Light");
	{
		const DirectionalLight* directional = light2->AddComponent<DirectionalLight>();
		const auto trans = light2->GetTransform();

		trans->Rotate(Eigen::Vector3f{ -1.f, -1.f,-1.f }.normalized(), 45.f);
	}

	GameObject* player = SceneManager::GetInstance().CreateGameObject(L"Player");
	{
		MeshRenderer* renderer = player->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);
		renderer->SetTexture(L"PlayerTextureTest.png");

		Transform* trans = player->GetTransform();
		trans->SetLocalPosition({ 0.0f, 3.0f, 0.0f });

		BoxCollider* collider = player->AddComponent<BoxCollider>();
		collider->SetSize({ 0.5f, 0.5f, 0.5f });

		RigidBody* rigid = player->AddComponent<RigidBody>();
		rigid->SetMass(10.0f);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_X, true);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_Y, true);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_Z, true);
	}

	GameObject* playerDitection = SceneManager::GetInstance().CreateGameObject(L"PlayerDitection");
	{
		Transform* transform = playerDitection->GetTransform();
		transform->SetParent(player->GetTransform());

		BoxCollider* collider = playerDitection->AddComponent<BoxCollider>();
		collider->SetSize({ 0.2f, 0.2f, 0.2f });
		collider->SetTrigger(true);
	}

	GameObject* controllerObject = SceneManager::GetInstance().CreateGameObject(L"Controller");
	{
		Transform* trans = controllerObject->GetTransform();
		trans->SetParent(player->GetTransform());

		Controller* controller = controllerObject->AddComponent<Controller>();
		controller->SetPlayer(player);
	}

	GameObject* camera = SceneManager::GetInstance().CreateGameObject(L"Camera");
	{
		Camera* cameraObject = camera->AddComponent<Camera>();
		cameraObject->SetMainCamera();

		Transform* transform = camera->GetTransform();
		transform->SetParent(controllerObject->GetTransform());
		transform->SetLocalPosition({ 0.f, 3.f, -15.f });

		auto cameraCollider = camera->AddComponent<BoxCollider>();
		cameraCollider->SetSize({ 0.5f, 0.5f, 0.5f });
		cameraCollider->SetDynamic(true);
	}

	GameObject* box = SceneManager::GetInstance().CreateGameObject(L"Box");
	{
		Transform* transform = box->GetTransform();
		transform->SetLocalPosition({0.f, 10.f, 10.f});
		transform->SetLocalScale({ 2.f, 2.f, 2.f });

		MeshRenderer* renderer = box->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);
		renderer->SetTexture(L"WoodTextureTest.png");

		const auto collider = box->AddComponent<BoxCollider>();
		collider->SetSize({ 1.f, 1.f, 1.f });

		const auto rigidbody = box->AddComponent<RigidBody>();
		rigidbody->SetMass(100.f);
	}

	GameObject* ground = SceneManager::GetInstance().CreateGameObject(L"Ground");
	{
		MeshRenderer* renderer = ground->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* trnans = ground->GetTransform();
		trnans->SetLocalScale({ 100.0f, 10.0f, 100.0f });

		BoxCollider* collider = ground->AddComponent<BoxCollider>();
		collider->SetSize({ 50.f, 5.f, 50.f });

		RigidBody* rigid = ground->AddComponent<RigidBody>();
		rigid->UseGravity(false);
		rigid->SetKinematic(true);
	}
}
