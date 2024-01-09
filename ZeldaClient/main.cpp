#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

namespace PurahEngine
{
	class BoxCollider;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	std::wstring gameName = L"test";

	std::string path = "PurahEngine.dll";
	HMODULE PurahEngineDll = ::LoadLibraryA(path.c_str());

	assert(PurahEngineDll != nullptr);

	auto CreateInitialize = reinterpret_cast<void (*)(_In_ HINSTANCE hInstance, LPCWSTR gameName, unsigned int width, unsigned int height)>(GetProcAddress(PurahEngineDll, "Initialize"));
	auto CreateRun = reinterpret_cast<void (*)(_In_ int nCmdShow)>(GetProcAddress(PurahEngineDll, "Run"));
	auto CreateFinalize = reinterpret_cast<void (*)()>(GetProcAddress(PurahEngineDll, "Finalize"));

	// DLL 함수를 찾을 수 없습니다.
	assert(!(CreateInitialize == nullptr || CreateRun == nullptr || CreateFinalize == nullptr));

	//PurahEngine::Initialize(hInstance, gameName.c_str(), 1920, 1080); 역할
	CreateInitialize(hInstance, gameName.c_str(), 1920, 1080);

	auto renderer = PurahEngine::GraphicsManager::GetInstance().GetRenderer();
	static TextureID textureID = TextureID::ID_NULL;
	static ModelID modelID = ModelID::ID_NULL;
	static CameraID cameraID = CameraID::ID_NULL;




	//PurahEngine::GameObject* box = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box01");

	//
	//auto boxTrans = box->GetComponent<PurahEngine::Transform>();
	//boxTrans->SetLocalPosition({ 0, 5, 0 });
	//
	//auto colliderBox = box->AddComponent<PurahEngine::BoxCollider>();
	//colliderBox->SetSize({ 0.5f, 0.5f, 0.5f });
	//
	//auto rigid = box->AddComponent<PurahEngine::RigidBody>();
	//rigid->SetMass(10.f);
	//rigid->SetKinematic(false);
	//
	//auto cube = box->AddComponent<PurahEngine::Renderer>();
	//
	//PurahEngine::GameObject* box3 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box03");
	//box3->GetComponent<PurahEngine::Transform>()->SetLocalPosition({ 1, 6, 0 });
	//
	//auto colliderBox3 = box3->AddComponent<PurahEngine::BoxCollider>();
	//colliderBox3->SetSize({ 0.5f, 0.5f, 0.5f });
	//
	//auto rigid3 = box3->AddComponent<PurahEngine::RigidBody>();
	//rigid3->SetMass(10.f);
	//rigid3->SetKinematic(false);
	//
	//auto cube3 = box3->AddComponent<PurahEngine::Renderer>();
	//
	//PurahEngine::GameObject* box4 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box04");
	//box4->GetComponent<PurahEngine::Transform>()->SetLocalPosition({ -0.5, 3, 0 });
	//
	//auto colliderBox4 = box4->AddComponent<PurahEngine::BoxCollider>();
	//colliderBox4->SetSize({ 0.5f, 0.5f, 0.5f });
	//
	//auto rigid4 = box4->AddComponent<PurahEngine::RigidBody>();
	//rigid4->SetMass(10.f);
	//rigid4->SetKinematic(false);
	//
	//auto cube4 = box4->AddComponent<PurahEngine::Renderer>();
	//
	//PurahEngine::GameObject* box2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box02");
	//
	//auto trans = box2->GetComponent<PurahEngine::Transform>();
	//trans->SetLocalPosition({0.f, -3.f, 0.f});
	//
	//auto colliderBox2 = box2->AddComponent<PurahEngine::BoxCollider>();
	//colliderBox2->SetSize({ 5, 0.5f, 5 });
	//box2->GetComponent<PurahEngine::Transform>()->SetLocalScale({ 10, 1, 10 });
	//auto rigid2 = box2->AddComponent<PurahEngine::RigidBody>();
	//rigid2->SetMass(10.f);
	//rigid2->SetKinematic(true);
	//
	//auto cube2 = box2->AddComponent<PurahEngine::Renderer>();
	//
	//
	//
	//colliderBox->Awake();
	//colliderBox2->Awake();
	//colliderBox3->Awake();
	//colliderBox4->Awake();
	//rigid->Awake();
	//rigid2->Awake();
	//rigid3->Awake();
	//rigid4->Awake();


	//auto boxTrans = box->GetComponent<PurahEngine::Transform>();
	//boxTrans->SetLocalPosition({ 0, 5, 0 });

	//auto boxCollider = box->AddComponent<PurahEngine::BoxCollider>();
	//boxCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	//auto rigid = box->AddComponent<PurahEngine::RigidBody>();
	//rigid->SetMass(10.f);
	//rigid->UseGravity(true);


	PurahEngine::GameObject* box2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box02");

	auto box2Trans = box2->GetComponent<PurahEngine::Transform>();
	box2Trans->SetLocalPosition({0.f, -5.f, 0.f});

	auto box2Collider = box2->AddComponent<PurahEngine::BoxCollider>();
	box2Collider->SetSize({ 100, 0.5f, 100 });

	auto rigid2 = box2->AddComponent<PurahEngine::RigidBody>();
	rigid2->SetMass(10.f);
	rigid2->UseGravity(false);
	rigid2->SetKinematic(true);



	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	testObject->AddComponent<PurahEngine::Renderer>();
	testObject->AddComponent<PurahEngine::TestMovement>();

	auto boxTrans = testObject->GetComponent<PurahEngine::Transform>();
	boxTrans->SetLocalPosition({ 0, 5, 0 });

	auto boxCollider = testObject->AddComponent<PurahEngine::BoxCollider>();
	boxCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	auto rigid = testObject->AddComponent<PurahEngine::RigidBody>();
	rigid->SetMass(10.f);
	rigid->UseGravity(true);

	boxCollider->Awake();
	box2Collider->Awake();
	rigid->Awake();
	rigid2->Awake();





	PurahEngine::GameObject* camera = PurahEngine::SceneManager::GetInstance().GetMainCamera()->GetGameObject();
	camera->AddComponent<PurahEngine::Camera>();
	camera->GetComponent<PurahEngine::Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
	camera->GetComponent<PurahEngine::Camera>()->SetRenderer(renderer);
	camera->GetComponent<PurahEngine::Camera>()->CreateCamera();
	camera->GetComponent<PurahEngine::Camera>()->SetMainCamera();
	camera->AddComponent<PurahEngine::CameraMovement>();


	//PurahEngine::GameObject* testObject2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject2");
	//testObject2->AddComponent<PurahEngine::Renderer>();
	//testObject2->GetComponent<PurahEngine::Transform>()->SetLocalPosition({ 2, 0, 0 });
	testObject->GetComponent<PurahEngine::Transform>()->SetLocalPosition({ -2, 0, 0 });
	//testObject->GetComponent<PurahEngine::Transform>()->SetParent(testObject2->GetComponent<PurahEngine::Transform>());
	float angle = 30.0f;
	//testObject2->GetComponent<PurahEngine::Transform>()->Rotate(Eigen::Vector3f(1, 1, 1).normalized(), angle);

	//PurahEngine::GameObject* testObject3 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject3");
	//testObject3->AddComponent<PurahEngine::Renderer>();
	//testObject3->GetComponent<PurahEngine::Transform>()->SetLocalPosition({ -2, 0, 0 });
	testObject->GetComponent<PurahEngine::Transform>()->Rotate(Eigen::Vector3f(1, 1, 1).normalized(), angle);
	//testObject->GetComponent<PurahEngine::Transform>()->SetParent(testObject3->GetComponent<PurahEngine::Transform>());



	auto colliderBox5 = testObject->AddComponent<PurahEngine::BoxCollider>();
	colliderBox5->SetSize({0.5f, 0.5f, 0.5f });
	
	colliderBox5->Awake();
	auto rigid5 = testObject->AddComponent<PurahEngine::RigidBody>();
	rigid5->SetMass(10.f);
	rigid5->Awake();
	//rigid5->SetPosition(testObject->GetComponent<PurahEngine::Transform>()->GetWorldPosition());
	


	//textureID = renderer->CreateTexture(L"scd.jpg");
	//if (textureID == TextureID::ID_NULL)
	//{
	//	assert(0);
	//}
	//testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);

	//cube->AddTexture(textureID);
	//cube2->AddTexture(textureID);
	//cube3->AddTexture(textureID);
	//cube4->AddTexture(textureID);

	textureID = renderer->CreateTexture(L"scd.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}

	// testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);
	auto cube = testObject->AddComponent<PurahEngine::Renderer>();
	auto cube2 = box2->AddComponent<PurahEngine::Renderer>();
	cube->AddTexture(textureID);
	cube2->AddTexture(textureID);


	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
