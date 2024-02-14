#include "NaDong.h"
#include <cassert>

#include "PurahEngine.h"

#include "TestMovement.h"
#include "CameraMovement.h"

using namespace PurahEngine;

void NaDong::Run()
{
	// 카메라 생성
	GameObject* mainCameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
	Camera* mainCamera = mainCameraObject->AddComponent<Camera>();

	// 메인 카메라로 설정
	SceneManager::GetInstance().SetMainCamera(mainCamera);

	// 라이트 생성
	GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"Light");
	DirectionalLight* light = lightObject->AddComponent<DirectionalLight>();
	
	// 라이트의 방향을 왼쪽 위에서 들여다 보는 방향으로 설정

	// 큐브 생성
	GameObject* cubeObject = SceneManager::GetInstance().CreateGameObject(L"Cube");
	MeshRenderer* cube = cubeObject->AddComponent<MeshRenderer>();

	// MeshRenderer의 타입을 Cube로 설정한다.
	// 기본값은 None으로 이 경우 아무것도 화면에 뜨지 않는다.(Debug 모드의 경우 런타임 에러를 발생시킨다.)
	cube->SetMesh(MeshRenderer::MeshType::Cube);




}
