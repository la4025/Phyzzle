#include <windows.h>
#include <iostream>
#include <cassert>
#include "PurahEngine.h"

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
	float angle = 10.0f;

	

	PurahEngine::GameObject* testObject = PurahEngine::SceneManager::GetInstance().CreateGameObject(L"testObject");
	PurahEngine::GameObject* camera = PurahEngine::SceneManager::GetInstance().GetMainCamera()->GetGameObject();
	camera->AddComponent<PurahEngine::Camera>();
	testObject->AddComponent<PurahEngine::Renderer>();

	camera->GetComponent<PurahEngine::Transform>()->SetLocalPosition(Eigen::Vector3f(0, 0, -10));
	camera->GetComponent<PurahEngine::Camera>()->SetRenderer(renderer);
	camera->GetComponent<PurahEngine::Camera>()->CreateCamera();
	camera->GetComponent<PurahEngine::Camera>()->SetMainCamera();


	textureID = renderer->CreateTexture(L"scd.jpg");
	if (textureID == TextureID::ID_NULL)
	{
		assert(0);
	}
	testObject->GetComponent<PurahEngine::Renderer>()->AddTexture(textureID);


	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('Q') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->Rotate(testObject->GetComponent<PurahEngine::Transform>()->up, angle);
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('E') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->Rotate(Eigen::Vector3f::UnitY(), -angle);
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('W') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalPosition(testObject->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, 0.1f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('S') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalPosition(testObject->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.0f, -0.1f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('A') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalPosition(testObject->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(-0.1f, 0.0f, 0.0f));
	}
	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('D') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalPosition(testObject->GetComponent<PurahEngine::Transform>()->GetLocalPosition() + Eigen::Vector3f(0.1f, 0.0f, 0.0f));
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('Z') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalScale(testObject->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseProduct(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}

	if (PurahEngine::InputManager::Getinstance().IsKeyPressed('C') == true)
	{
		testObject->GetComponent<PurahEngine::Transform>()->SetLocalScale(testObject->GetComponent<PurahEngine::Transform>()->GetLocalScale().cwiseQuotient(Eigen::Vector3f(1.1f, 1.1f, 1.1f)));
	}


	//PurahEngine::Run(); 역할
	CreateRun(nCmdShow);
	//PurahEngine::Finalize(); 역할
	CreateFinalize();

	::FreeLibrary(PurahEngineDll);

	return 0;
}
