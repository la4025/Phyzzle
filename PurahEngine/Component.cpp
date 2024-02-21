#include "Collider.h"
#include "ZnCollision.h"
#include "Component.h"

PurahEngine::Component::Component() : gameObject(nullptr)
{

}

PurahEngine::Component::~Component()
= default;

void PurahEngine::Component::Awake()
{

}

void PurahEngine::Component::Start()
{

}

void PurahEngine::Component::FixedUpdate()
{

}

void PurahEngine::Component::Update()
{

}

void PurahEngine::Component::LateUpdate()
{

}

void PurahEngine::Component::OnCollisionEnter(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnCollisionStay(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnCollisionExit(const ZonaiPhysics::ZnCollision& collision, const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnTriggerEnter(const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnTriggerStay(const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnTriggerExit(const PurahEngine::Collider* collider)
{

}

void PurahEngine::Component::OnMouseEnter()
{

}

void PurahEngine::Component::OnMouseStay()
{

}

void PurahEngine::Component::OnMouseExit()
{

}

void PurahEngine::Component::Initialize()
{

}

PurahEngine::GameObject* PurahEngine::Component::GetGameObject() const
{
	return gameObject;
}
