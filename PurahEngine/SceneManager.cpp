#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "TimeController.h"
#include "EngineSetting.h"
#include "DataManager.h"

PurahEngine::SceneManager::SceneManager()
	: mainCamera(nullptr), cameraPosition(Eigen::Vector3f(0.0f, 0.0f, -10.0f)), state(RunningState::AWAKE), physicsTime(0.0f), sceneBuffer(L"")
{

}

PurahEngine::SceneManager::~SceneManager()
{

}

PurahEngine::GameObject* PurahEngine::SceneManager::CreateGameObject(std::wstring objectName)
{
	GameObject* object = new GameObject(objectName);
	objectList[objectName] = object;
	object->trans = object->AddComponentInit<Transform>();
	return object;
}

PurahEngine::Camera* PurahEngine::SceneManager::GetMainCamera()
{
	return mainCamera;
}

void PurahEngine::SceneManager::SetMainCamera(Camera* camera)
{
	mainCamera = camera;
}

void PurahEngine::SceneManager::SetName(std::wstring name)
{
	sceneName = name;
}

void PurahEngine::SceneManager::Update()
{
	PurahEngine::TimeController::GetInstance().Update("Simulate");
	float SdeltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("Simulate");
	static float SimuleTime = 0;
	SimuleTime += SdeltaTime;

	PurahEngine::TimeController::GetInstance().Update("physics");
	float deltaTime = PurahEngine::TimeController::GetInstance().GetDeltaTime("physics");
	physicsTime += deltaTime;

	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->UpdateEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->LateUpdateEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	if (physicsTime >= 0.02f)
	{
		for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
		{
			PurahEngine::GameObject* object = iter->second;
			if (object->trans->GetParent() == nullptr)
			{
				object->FixedUpdateEvent(eventQueue);
			}
		}
	}

	if (physicsTime >= 0.02f)
	{
		physicsTime -= 0.02f;
	}
}

void PurahEngine::SceneManager::LoadScene(const std::wstring fileName)
{
	sceneBuffer = fileName;
}

void PurahEngine::SceneManager::LoadScene(int sceneNumber)
{
	sceneBuffer = EngineSetting::GetInstance().GetScene(sceneNumber);
}

void PurahEngine::SceneManager::DeleteGameObject(GameObject* gameObject)
{
	auto objectIter = objectList.find(gameObject->GetName());

	if (objectIter != objectList.end())
	{
		delete objectIter->second;
		objectList.erase(objectIter);
	}

	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		iter->second->DeleteChild(gameObject);
	}
}

void PurahEngine::SceneManager::InitializationEvent()
{
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->PostInitializeEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// Awake
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->AwakeEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// OnEnable(활성화 직 후)
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->EnableEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// Start
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->StartEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// State Change
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->StateChangeEvent();
		}
	}
}

void PurahEngine::SceneManager::DecommissionEvent()
{
	// OnDisable (비활성화 상태)
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->DisableEvent(eventQueue);
		}
	}
	ExcuteEventQueue();

	// OnDestroy (맨 마지막프레임에 오브젝트 파괴)
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		if (object->trans->GetParent() == nullptr)
		{
			object->DestroyEvent(destroyQueue);
		}
	}
	ExcuteDestroyQueue();
}

void PurahEngine::SceneManager::ExcuteEventQueue()
{
	while (!eventQueue.empty())
	{
		Component* component = eventQueue.front().first;
		auto f = eventQueue.front().second;
		eventQueue.pop();

		f(*component);
	}
}

void PurahEngine::SceneManager::ExcuteDestroyQueue()
{
	while (!destroyQueue.empty())
	{
		GameObject* object = destroyQueue.front();
		destroyQueue.pop();

		DeleteGameObject(object);
	}
}

void PurahEngine::SceneManager::LoadScene()
{
	if (sceneBuffer == L"")
	{
		return;
	}

	auto& fManager = PurahEngine::FileManager::GetInstance();
	fManager.clear();
	DataManager::GetInstance().ClearDontDestroy();

	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		bool checkDontDestroy = iter->second->GetDontDestroy();
		PurahEngine::GameObject* object = iter->second;

		if (!checkDontDestroy)
		{
			delete object;
		}
		else
		{
			DataManager::GetInstance().DontDestroyOnLoad(object);
		}
	} 


	objectList.clear();
	sceneData = fManager.LoadData(sceneBuffer);
	Deserialize(sceneData);

	LoadSceneCompleteEvent();

	LoadDontDestroyObject();

	// 필요하다면 여기서 sceneName 변경하는 코드 추가
	sceneBuffer = L"";
}

void PurahEngine::SceneManager::LoadSceneCompleteEvent()
{
	for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	{
		PurahEngine::GameObject* object = iter->second;
		int componentSize = object->componentList.size();

		for (int i = 0; i < componentSize; i++)
		{
			object->componentList[i]->OnDataLoadComplete();
		}
	}
}

void PurahEngine::SceneManager::LoadDontDestroyObject()
{
	std::map<std::wstring, GameObject*> dontDestroyList = DataManager::GetInstance().dontDestroyObjectList;
	int size = dontDestroyList.size();
	if (size != 0)
	{
		for (auto iter = dontDestroyList.begin(); iter != dontDestroyList.end(); iter++)
		{
			objectList[iter->first] = iter->second;
		}
	}
}

void PurahEngine::SceneManager::PreSerialize(json& jsonData) const
{

}

void PurahEngine::SceneManager::PreDeserialize(const json& jsonData)
{
	for (int i = 0; i < jsonData["gameObjects"].size(); i++)
	{
		std::string name = jsonData["gameObjects"][i]["name"];
		GameObject* object = CreateGameObject(std::wstring(name.begin(), name.end()));
		object->PreDeserialize(jsonData["gameObjects"][i]);
	}
}

void PurahEngine::SceneManager::PostSerialize(json& jsonData) const
{

}

void PurahEngine::SceneManager::PostDeserialize(const json& jsonData)
{

	//for (int i = 0; i < objectList.size(); i++)
	//{
	//	objectList[i]->PostDeserialize(jsonData["gameObjects"][i]);
	//}
	//
	//for (auto iter = objectList.begin(); iter != objectList.end(); iter++)
	//{
	//	PurahEngine::GameObject* object = iter->second;
	//	for (int i = 0; i < jsonData["gameObjects"].size(); i++)
	//	{
	//		std::wstring name = jsonData["gameObjects"][i]["name"];
	//		if (name == iter->first)
	//		{
	//			object->PostDeserialize(jsonData["gameObjects"][i]);
	//		}
	//	}
	//}

	for (int i = 0; i < jsonData["gameObjects"].size(); i++)
	{
		std::string name = jsonData["gameObjects"][i]["name"];
		std::wstring wname(name.begin(), name.end());
		PurahEngine::GameObject* object = objectList[wname];
		object->PostDeserialize(jsonData["gameObjects"][i]);
	}

}

void PurahEngine::SceneManager::Initialize()
{
	// 씬을 초기화할때 카메라를 씬에 생성해둔다.
	if (mainCamera == nullptr)
	{
		GameObject* object = CreateGameObject(L"MainCamera");
		Camera* tmp = object->AddComponentInit<Camera>();
		mainCamera = tmp;
		physicsTime = 0.0f;
	}
}

PurahEngine::SceneManager& PurahEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}