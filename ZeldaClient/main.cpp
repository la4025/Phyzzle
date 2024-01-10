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

	// camera
	{
		PurahEngine::GameObject* camera = PurahEngine::SceneManager::GetInstance().GetMainCamera()->GetGameObject();
		camera->AddComponent<PurahEngine::Camera>();
		camera->GetComponent<PurahEngine::Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
		camera->GetComponent<PurahEngine::Camera>()->SetRenderer(renderer);
		camera->GetComponent<PurahEngine::Camera>()->CreateCamera();
		camera->GetComponent<PurahEngine::Camera>()->SetMainCamera();
		camera->AddComponent<PurahEngine::CameraMovement>();
	}

#pragma region testObject
	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	testObject->AddComponent<PurahEngine::Renderer>();
	testObject->AddComponent<PurahEngine::TestMovement>();

	auto objTrans = testObject->GetComponent<PurahEngine::Transform>();
	objTrans->SetLocalPosition({ 0.0f, 5.0f, 0.0f });

	auto objCollider = testObject->AddComponent<PurahEngine::BoxCollider>();
	objCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	auto objRigid = testObject->AddComponent<PurahEngine::RigidBody>();
	objRigid->SetMass(10.f);
	objRigid->UseGravity(true);

	objCollider->Awake();
	objRigid->Awake();
#pragma endregion


#pragma region box1
	PurahEngine::GameObject* box = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box01");
	box->AddComponent<PurahEngine::Renderer>();

	auto boxTrans = box->GetComponent<PurahEngine::Transform>();
	boxTrans->SetLocalPosition({ 2.0f, 3.0f, 0.0f });
	boxTrans->SetLocalScale({ 1.0f, 1.0f, 1.0f });

	auto boxCollider = box->AddComponent<PurahEngine::BoxCollider>();
	boxCollider->SetSize({ 0.5f, 0.5f, 0.5f });

	auto rigid = box->AddComponent<PurahEngine::RigidBody>();
	rigid->SetMass(10.f);
	rigid->UseGravity(true);
	rigid->SetKinematic(false);

	boxCollider->Awake();
	rigid->Awake();
#pragma endregion

#pragma region plane
	PurahEngine::GameObject* box2 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box02");
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
#pragma endregion

#pragma region box3
	PurahEngine::GameObject* box3 = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"Box03");
	box3->AddComponent<PurahEngine::Renderer>();

	auto box3Trans = box3->GetComponent<PurahEngine::Transform>();
	box3Trans->SetLocalPosition({ -2.0f, 0.0f, 0.0f });
	box3Trans->SetLocalScale({ 1.0f, 1.0f, 1.0f });
	box3Trans->SetParent(objTrans);

	auto box3Collider = box3->AddComponent<PurahEngine::BoxCollider>();
	box3Collider->SetSize({ 0.5f, 0.5f, 0.5f });

	auto rigid3 = box3->AddComponent<PurahEngine::RigidBody>();
	rigid3->SetMass(10.f);
	rigid3->UseGravity(true);
	rigid3->SetKinematic(false);

	box3Collider->Awake();
	rigid3->Awake();
#pragma endregion





	textureID = renderer->CreateTexture(L"scd.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}

	//cube2->AddTexture(textureID);


	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
