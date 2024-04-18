#include "ZnCollider.h"
#include "ZnCollision.h"

#include "Component.h"
#include "EngineSetting.h"

#include "GameObject.h"

void PurahEngine::GameObject::AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::Awake) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}

		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
		}
	}
}

void PurahEngine::GameObject::EnableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::OnEnable) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, false);
			}
		}
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::OnEnable) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->EnableEvent(eventQueue, true);
			}
		}
	}
}

void PurahEngine::GameObject::StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::Start) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}

		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->AwakeEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->StartEvent(eventQueue, false);
		}
	}
}

void PurahEngine::GameObject::FixedUpdateEvent()
{
	if (state == ObjectState::ENABLE)
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
	if (state == ObjectState::ENABLE)
	{
		for (PurahEngine::Component* component : componentList)
		{
			component->LateUpdate();
		}

		for (int i = 0; i < trans->GetChildren().size(); i++)
		{
			trans->GetChildren()[i]->GetGameObject()->LateUpdateEvent();
		}
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
	SetEnable(false);
	isDestroy = true;
	if (state != ObjectState::ENABLE)
	{
		state = ObjectState::DESTROY;
	}

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->Destroy();
	}
}

void PurahEngine::GameObject::DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& eventQueue, bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::OnDisable) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, false);
			}
		}
		if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
		else
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				eventQueue.push({ componentList[i], (&Component::OnDisable) });
			}

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->DisableEvent(eventQueue, true);
			}
		}
	}
}

void PurahEngine::GameObject::DestroyEvent(std::queue<GameObject*>& destroyQueue)
{
	if (state == ObjectState::DESTROY)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnDestroy();
		}
	}

	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		trans->GetChildren()[i]->GetGameObject()->DestroyEvent(destroyQueue);
	}

	if (state == ObjectState::DESTROY)
	{
		destroyQueue.push(this);
	}
}

void PurahEngine::GameObject::StateChangeEvent(bool parentEnable)
{
	if (state == ObjectState::CREATE)
	{
		if (parentEnable == true && isEnable == true)
		{
			state = ObjectState::ENABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}
		}
	}
	else if (state == ObjectState::ENABLE)
	{
		if (isDestroy == true)
		{
			state = ObjectState::DESTROY;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(false);
			}
		}
		else if (parentEnable == true && isEnable == true)
		{
			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}
		}
		else
		{
			state = ObjectState::DISABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(false);
			}
		}
	}
	else if (state == ObjectState::DISABLE)
	{
		if (isEnable == true)
		{
			state = ObjectState::ENABLE;

			for (int i = 0; i < trans->GetChildren().size(); i++)
			{
				trans->GetChildren()[i]->GetGameObject()->StateChangeEvent(true);
			}
		}
	}
}

PurahEngine::Transform* PurahEngine::GameObject::GetTransform()
{
	return trans;
}

std::vector<PurahEngine::Component*> PurahEngine::GameObject::GetComponents()
{
	return componentList;
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
	if (isDestroy == true && isEnable == true)
	{
		return;
	}

	isEnable = isTrue;
}

bool PurahEngine::GameObject::GetEnable()
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
		rootEnable = GetStateEnable();
	}

	bool ownEnable = GetStateEnable() && rootEnable;

	return ownEnable;
}

std::wstring PurahEngine::GameObject::GetName()
{
	return name;
}

bool PurahEngine::GameObject::GetStateEnable()
{
	return state == ObjectState::ENABLE;
}

void PurahEngine::GameObject::PreSerialize(json& jsonData) const
{

}

void PurahEngine::GameObject::PreDeserialize(const json& jsonData)
{
	auto& fManager = FileManager::GetInstance();
	fManager.SetAddress(jsonData["__Base__instanceID"], this);
	trans->PreDeserialize(jsonData["transform"]);
	bool active = jsonData["active"];
	SetEnable(active);
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
	isDestroy = false;
}

PurahEngine::GameObject::GameObject(std::wstring objectname, bool isenable)
	: trans(nullptr)
{
	state = ObjectState::CREATE;
	name = objectname;
	isEnable = isenable;
	isDestroy = false;
}

PurahEngine::GameObject::~GameObject()
{
	for (int i = 0; i < trans->GetChildren().size(); i++)
	{
		delete trans->GetChildren()[i]->GetGameObject();
	}

	for (int i = 0; i < componentList.size(); i++)
	{
		delete componentList[i];
	}
	componentList.clear();
}

PurahEngine::GameObject::ObjectState PurahEngine::GameObject::GetState()
{
	return state;
}

PurahEngine::Component* PurahEngine::GameObject::AddComponentToString(std::string componentName)
{
	Component* component = ComponentFactory::GetInstance().componentFactory[componentName]();
	componentList.push_back(component);
	component->gameObject = this;
	component->Initialize();

	return component; // 추가된 컴포넌트 포인터를 반환
}
