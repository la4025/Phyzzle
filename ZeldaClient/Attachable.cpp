#include "Attachable.h"


namespace Phyzzle
{
	Attachable::~Attachable()
	{
	}

	void Attachable::Start()
	{
		body = GetGameObject()->GetComponent<PurahEngine::RigidBody>();

		assert(body != nullptr);
	}

	IslandID Attachable::GetIslandID() const
	{
		return islandID;
	}

	void Attachable::ValiantStore()
	{
		isKinematic = body->IsKinematic();
		hasGravity = body->HasGravity();
		originMass = body->GetMass();
	}

	void Attachable::Selected() const
	{
		body->SetKinematic(false);
		body->UseGravity(false);
		body->SetMass(0.001f);
	}

	void Attachable::ValiantRetrieve()
	{
		body->SetKinematic(isKinematic);
		body->UseGravity(hasGravity);
		body->SetMass(originMass);

		isKinematic = false;
		hasGravity = false;
		originMass = -1.f;
	}

	void Attachable::OnCollisionEnter(
		const ZonaiPhysics::ZnCollision& _collision, 
		const PurahEngine::Collider* _collider)
	{
		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			body = obj->GetComponent<PurahEngine::RigidBody>();
		}
	}

	void Attachable::OnCollisionExit(
		const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			if (body == obj->GetComponent<PurahEngine::RigidBody>())
			{
				body = nullptr;
			}
		}
	}
}
