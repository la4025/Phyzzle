#include "PzObject.h"

#include "AttachSystem.h"


namespace Phyzzle
{
	PzObject::~PzObject()
	{
	}

	void PzObject::Start()
	{
		body = GetGameObject()->GetComponent<PurahEngine::RigidBody>();

		assert(body != nullptr);
	}

	void PzObject::Update()
	{
		if (attachable)
		{
			bool enter = !preState && select;
			bool stay = preState && select;
			bool exit = preState && !select;

			if (enter || stay)
			{
				AttachSystem::Instance()->EnableOutline(attachable, true, PurahColor::Yellow);
			}
			else if (exit)
			{
				AttachSystem::Instance()->EnableOutline(attachable, false);
			}
		}

		preState = select;
	}

	//bool PzObject::isLocked()
	//{
	//}

	IslandID PzObject::GetIslandID() const
	{
		return islandID;
	}

	void PzObject::ValiantStore()
	{
		isKinematic = body->IsKinematic();
		hasGravity = body->HasGravity();
		originMass = body->GetMass();

		assert(originMass > 0.01f);
	}

	void PzObject::Selected()
	{
		if (select)
			return;

		ValiantStore();

		body->SetKinematic(false);
		body->UseGravity(false);
		body->SetMass(1.f);

		select = true;
	}

	void PzObject::ValiantRetrieve()
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

	void PzObject::OnCollisionEnter(
		const ZonaiPhysics::ZnCollision& _collision, 
		const PurahEngine::Collider* _collider)
	{
		if (attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();
		
		if (obj->tag.IsContain(L"Attachable"))
		{
			attachable = obj->GetComponent<PzObject>();

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

	void PzObject::OnCollisionStay(const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (attachable == obj->GetComponent<PzObject>())
		{
			Eigen::Vector3f acc = Eigen::Vector3f::Zero();
			for (int i = 0; i < _collision.contactCount; i++)
			{
				acc += _collision.contacts[i].point;
			}
			worldAnchor = (acc / _collision.contactCount);
		}
	}

	void PzObject::OnCollisionExit(
		const ZonaiPhysics::ZnCollision& _collision,
		const PurahEngine::Collider* _collider)
	{
		if (!attachable)
			return;

		PurahEngine::GameObject* obj = _collider->GetGameObject();

		if (obj->tag.IsContain(L"Attachable"))
		{
			if (attachable == obj->GetComponent<PzObject>())
			{
				if (select)
				{
					AttachSystem::Instance()->EnableOutline(attachable, false);
				}

				attachable = nullptr;
				worldAnchor = Eigen::Vector3f::Zero();
			}
		}
	}

	void PzObject::PreDeserialize(const json& jsonData)
	{
		// PREDESERIALIZE_BASE();
	}

	void PzObject::PostDeserialize(const json& jsonData)
	{
		//const json& materials = jsonData["connectedObject"];
		//size_t connectedCount = materials.size();
		//std::vector<PzObject*> deserialize(connectedCount);

		//for (size_t i = 0; i < connectedCount; i++)
		//{
		//	PzObject* connectedObject;
		//	POSTDESERIALIZE_PTR(connectedObject);
		//	deserialize[i] = connectedObject;
		//}
	}
}
