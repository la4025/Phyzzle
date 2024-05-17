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

		assert(originMass > 0.01f);
	}

	void Attachable::Selected()
	{
		if (select)
			return;

		ValiantStore();

		body->SetKinematic(false);
		body->UseGravity(false);
		body->SetMass(1.f);

		select = true;
	}

	void Attachable::ValiantRetrieve()
	{
		if (!select)
			return;

		assert(originMass > 0.01f);

		body->SetKinematic(isKinematic);
		body->UseGravity(hasGravity);
		body->SetMass(originMass);

		isKinematic = false;
		hasGravity = false;
		originMass = -1.f;

		select = false;
	}

	void Attachable::OnCollisionEnter(
		const ZonaiPhysics::ZnCollision& _collision, 
		const PurahEngine::Collider* _collider)
	{
		if (attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			attachable = obj->GetComponent<Attachable>();

			const bool otherIDNull = attachable->islandID == nullptr;
			const bool myIDNull = islandID == nullptr;
			const bool sameNull = otherIDNull && myIDNull;
			const bool sameID = attachable->islandID == islandID;
			bool diffID = !sameNull && sameID;					// 둘 다 널이 아니고 

			if (diffID)
			{
				attachable = nullptr;
				return;
			}

			// 앵커 위치
			Eigen::Vector3f acc = Eigen::Vector3f::Zero();
			for (int i = 0; i < _collision.contactCount; i++)
			{
				acc += _collision.contacts[i].point;
			}
			worldAnchor = (acc / _collision.contactCount);
		}
	}

	void Attachable::OnCollisionStay(const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (attachable == obj->GetComponent<Attachable>())
		{
			Eigen::Vector3f acc = Eigen::Vector3f::Zero();
			for (int i = 0; i < _collision.contactCount; i++)
			{
				acc += _collision.contacts[i].point;
			}
			worldAnchor = (acc / _collision.contactCount);
		}
	}

	void Attachable::OnCollisionExit(
		const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			if (attachable == obj->GetComponent<Attachable>())
			{
				attachable = nullptr;

				worldAnchor = Eigen::Vector3f::Zero();
			}
		}
	}
}
