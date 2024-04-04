#include "ZnCollider.h"
#include "ZnCollision.h"

#include "Component.h"
#include "EngineSetting.h"

#include "GameObject.h"

void PurahEngine::GameObject::AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue)
{
	if (state == ObjectState::CREATE)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			eventQueue.push({ componentList[i], (&Component::Awake) });
		}

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue);
		}
	}
}

void PurahEngine::GameObject::OnEnable()
{
	if (isEnable == true)
	{
		Enable();
	}
	else
	{
		Disable();
	}
}

void PurahEngine::GameObject::StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue)
{
	if (state == ObjectState::CREATE)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			eventQueue.push({ componentList[i], (&Component::Start) });
		}

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue);
		}
	}
}

void PurahEngine::GameObject::FixedUpdateEvent()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->FixedUpdate();
	}

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->FixedUpdateEvent();
	}
}

void PurahEngine::GameObject::UpdateEvent()
{
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			component->Update();
		}

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->UpdateEvent();
		}
	}
}

void PurahEngine::GameObject::LateUpdateEvent()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->LateUpdate();
	}
}

void PurahEngine::GameObject::OnDisable()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnDisable();
	}
}

void PurahEngine::GameObject::OnDestroy()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnDestroy();
	}
}

void PurahEngine::GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		if (trans->GetChildren()[i]->GetGameObject() == child)
		{
			delete trans->GetChildren()[i]->GetGameObject();
			trans->GetChildren().erase(trans->GetChildren().begin() + i);
		}

		trans->GetChildren()[i]->GetGameObject()->DeleteChild(child);
	}
}

void PurahEngine::GameObject::Destroy()
{
	state == ObjectState::DESTROY;

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->Destroy();
	}
}

void PurahEngine::GameObject::Enable()
{
	state = ObjectState::ENABLE;
	isEnable = true;
}

void PurahEngine::GameObject::Disable()
{
	state = ObjectState::DISABLE;
	isEnable = false;
}

bool PurahEngine::GameObject::IsEnable()
{
	return isEnable;
}

bool PurahEngine::GameObject::IsRootEnable()
{
	bool rootEnable = true;

	if (trans->GetParent() != nullptr)
	{
		rootEnable = trans->GetParent()->GetGameObject()->IsRootEnable();
	}
	else
	{
		rootEnable = IsEnable();
	}

	bool ownEnable = IsEnable() && rootEnable;

	return ownEnable;
}

PurahEngine::Transform* PurahEngine::GameObject::GetTransform()
{
	return trans;
}

void PurahEngine::GameObject::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionEnter(collision, collider);
	}
}

void PurahEngine::GameObject::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionStay(collision, collider);
	}
}

void PurahEngine::GameObject::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnCollisionExit(collision, collider);
	}
}

void PurahEngine::GameObject::OnTriggerEnter(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerEnter(collider);
	}
}

void PurahEngine::GameObject::OnTriggerStay(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerStay(collider);
	}
}

void PurahEngine::GameObject::OnTriggerExit(const Collider* collider)
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnTriggerExit(collider);
	}
}

void PurahEngine::GameObject::OnMouseEnter()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseEnter();
	}
}

void PurahEngine::GameObject::OnMouseStay()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseStay();
	}
}

void PurahEngine::GameObject::OnMouseExit()
{
	for (PurahEngine::Component* component : componentList)
	{
		component->OnMouseExit();
	}
}

void PurahEngine::GameObject::SetEnable(bool isTrue)
{
	isEnable = isTrue;
}

std::wstring PurahEngine::GameObject::GetName()
{
	return name;
}

void PurahEngine::GameObject::PreSerialize(json& jsonData) const
{

}

void PurahEngine::GameObject::PreDeserialize(const json& jsonData)
{
	auto& fManager = FileManager::GetInstance();
	fManager.SetAddress(jsonData["__Base__instanceID"], this);
	trans->PreDeserialize(jsonData["transform"]);
	for (int i = 0; i < jsonData["components"].size(); i++)
	{
		Component* component = AddComponentToString(jsonData["components"][i]["__Base__Component"]);
		component->PreDeserialize(jsonData["components"][i]);
	}

	std::vector<std::wstring> tags;
	for (int i = 0; i < jsonData["tag"].size(); i++)
	{
		std::string tagName = jsonData["tag"][i];
		std::wstring wtagName(tagName.begin(), tagName.end());
		tags.push_back(wtagName);
	}
	tag = tags;
}

void PurahEngine::GameObject::PostSerialize(json& jsonData) const
{

}

void PurahEngine::GameObject::PostDeserialize(const json& jsonData)
{
	int count = 0;
	trans->PostDeserialize(jsonData["transform"]);
	for (int i = 0; i < componentList.size(); i++)
	{
		if (dynamic_cast<Transform*>(componentList[i]) == nullptr)
		{
			componentList[i]->PostDeserialize(jsonData["components"][count]);
			count++;
		}
	}
}

PurahEngine::GameObject::GameObject(std::wstring objectname)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = true;
}

PurahEngine::GameObject::GameObject(std::wstring objectname, bool isenable)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = isenable;
}

PurahEngine::GameObject::~GameObject()
{
	for (int i = 0; i < componentList.size(); i++)
	{
		delete componentList[i];
	}
	componentList.clear();
}

PurahEngine::Component* PurahEngine::GameObject::AddComponentToString(std::string componentName)
{
	Component* component = ComponentFactory::GetInstance().componentFactory[componentName]();
	componentList.push_back(component);
	component->gameObject = this;
	component->Initialize();

	return component; // 추가된 컴포넌트 포인터를 반환
}
