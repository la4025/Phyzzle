#include "KiHoon.h"
#include <cassert>

#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

void KiHoon::Run()
{
	using namespace PurahEngine;

	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;
	static LightID lightID = LightID::ID_NULL;

	GameObject* camera = SceneManager::GetInstance().GetMainCamera()->GetGameObject();
	{
		const auto cameraObject = camera->AddComponent<PurahEngine::Camera>();
		camera->GetComponent<Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
		cameraObject->SetMainCamera();
		camera->AddComponent<CameraMovement>();
	}

#pragma region testObject
	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	{
		testObject->AddComponent<PurahEngine::Renderer>();
		testObject->AddComponent<PurahEngine::TestMovement>();

		auto objTrans = testObject->GetComponent<PurahEngine::Transform>();
		objTrans->SetLocalPosition({ 0.0f, 3.0f, 0.0f });


		auto objCollider = testObject->AddComponent<PurahEngine::BoxCollider>();
		objCollider->SetSize({ 0.5f, 0.5f, 0.5f });

		auto objRigid = testObject->AddComponent<PurahEngine::RigidBody>();
		objRigid->SetMass(10.0f);
		objRigid->UseGravity(true);

		objCollider->Awake();
		objRigid->Awake();
	}

#pragma endregion


#pragma region plane
	PurahEngine::GameObject* box2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box02");
	{
		box2->AddComponent<PurahEngine::Renderer>();

		auto box2Trans = box2->GetComponent<PurahEngine::Transform>();
		box2Trans->SetLocalPosition({ 0.f, -5.f, 0.f });
		box2Trans->SetLocalScale({ 50.0f, 1.0f, 50.0f });

		auto box2Collider = box2->AddComponent<PurahEngine::BoxCollider>();
		box2Collider->SetSize({ 100, 0.5f, 100 });

		auto rigid2 = box2->AddComponent<PurahEngine::RigidBody>();
		rigid2->SetMass(10.f);
		rigid2->UseGravity(false);
		rigid2->SetKinematic(true);

		box2Collider->Awake();
		rigid2->Awake();
	}
#pragma endregion

#pragma region box3
	PurahEngine::GameObject* box3 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box03");
	{
		const auto trans = testObject->GetComponent<PurahEngine::Transform>();

		box3->AddComponent<PurahEngine::Renderer>();

		const auto box3Trans = box3->GetComponent<PurahEngine::Transform>();
		box3Trans->SetParent(trans);
		box3Trans->SetLocalPosition({ -2.0f, 0.0f, 0.0f });
		box3Trans->SetLocalScale({ 1.0f, 1.0f, 1.0f });

		const auto box3Collider = box3->AddComponent<PurahEngine::BoxCollider>();
		box3Collider->SetDynamic(true);
		box3Collider->SetSize({ 0.5f, 0.5f, 0.5f });

		box3Collider->Awake();
	}
#pragma endregion


	//objTrans->SetParent(boxTrans);

	// CreateTexture의 사진파일은 zeldaClient 폴더에 넣어둘 것
	textureID = renderer->CreateTexture(L"mangGom4.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}

	testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);

	textureID = renderer->CreateTexture(L"mang.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}
	box3->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);


	PurahEngine::GameObject* light = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"light");
	light->AddComponent<PurahEngine::Renderer>();

	lightID = renderer->CreateDirectionalLight(Eigen::Vector3f(0.2f, 0.2f, 0.2f), Eigen::Vector3f(1.0f, 1.0f, 1.0f), Eigen::Vector3f(1.0f, 1.0f, 1.0f), Eigen::Vector3f(1.0f, 1.0f, 1.0f));

	light->GetComponent<PurahEngine::Renderer>()->AddLight(lightID);

}
