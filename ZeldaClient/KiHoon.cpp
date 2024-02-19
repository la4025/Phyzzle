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
		DirectionalLight* directional = light->AddComponent<DirectionalLight>();
		auto trans = light->GetTransform();

		//const Eigen::AngleAxisf rotation{ 1.f, Eigen::Vector3f{1.f, 0.f ,0.f} };
		//const Eigen::Quaternionf q{ rotation };
		//trans->SetWorldRotation(q);

		trans->Rotate({ 0, 1, 0 }, 45);
	}

	GameObject* camera = SceneManager::GetInstance().CreateGameObject(L"Camera");
	{
		Camera* cameraObject = camera->AddComponent<Camera>();
		SceneManager::GetInstance().SetMainCamera(cameraObject);
		camera->GetComponent<Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
	}

	GameObject* player = SceneManager::GetInstance().CreateGameObject(L"Player");
	{
		MeshRenderer* renderer = player->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* trans = player->GetTransform();
		trans->SetLocalPosition({ 0.0f, 3.0f, 0.0f });
		trans->SetLocalScale({ 1.f, 2.f, 1.f });

		BoxCollider* collider = player->AddComponent<BoxCollider>();
		collider->SetSize({ 0.5f, 1.f, 0.5f });

		RigidBody* rigid = player->AddComponent<RigidBody>();
		rigid->SetMass(10.0f);
		rigid->UseGravity(true);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_X, true);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_Y, true);
		rigid->SetDynamicLockFlag(ZonaiPhysics::LOCK_ANGULAR_Z, true);

		Controller* controller = player->AddComponent<Controller>();
	}

	{
		const auto transform = camera->GetTransform();
		transform->SetParent(player->GetTransform());
		transform->SetLocalPosition({ 0.f, 3.f, -15.f });

		auto cameraCollider = camera->AddComponent<BoxCollider>();
		cameraCollider->SetSize({ 0.5f, 0.5f, 0.5f });
		cameraCollider->SetDynamic(true);
	}

	GameObject* ground = SceneManager::GetInstance().CreateGameObject(L"Ground");
	{
		MeshRenderer* renderer = ground->AddComponent<MeshRenderer>();
		renderer->SetMesh(MeshRenderer::MeshType::Cube);

		Transform* trnans = ground->GetTransform();
		trnans->SetLocalPosition({ 0.f, -5.f, 0.f });
		trnans->SetLocalScale({ 100.0f, 1.0f, 100.0f });

		BoxCollider* collider = ground->AddComponent<BoxCollider>();
		collider->SetSize({ 50.f, 0.5f, 50.f });

		RigidBody* rigid = ground->AddComponent<RigidBody>();
		rigid->SetMass(10.f);
		rigid->UseGravity(false);
		rigid->SetKinematic(true);
	}
}
