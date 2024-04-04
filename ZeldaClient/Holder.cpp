#include "RigidBody.h"

#include "Holder.h"

#include "Collider.h"
#include "GameObject.h"

namespace Phyzzle
{
	void Holder::OnTriggerEnter(const PurahEngine::Collider* collider)
	{
		//if (collider->GetGameObject()->tag)
		//{
		//	
		//}
		//rigidbody = collider->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
	}

	void Holder::OnTriggerStay(const PurahEngine::Collider* collider)
	{

	}

	void Holder::OnTriggerExit(const PurahEngine::Collider* collider)
	{
		rigidbody = nullptr;
	}

	void Holder::OnCollisionEnter(
		const ZonaiPhysics::ZnCollision& zn_collision, 
		const PurahEngine::Collider* collider)
	{

	}

	void Holder::OnCollisionStay(
		const ZonaiPhysics::ZnCollision& zn_collision, 
		const PurahEngine::Collider* collider)
	{

	}

	void Holder::OnCollisionExit(
		const ZonaiPhysics::ZnCollision& zn_collision, 
		const PurahEngine::Collider* collider)
	{

	}

	PurahEngine::RigidBody* Holder::GetHolderableBody()
	{
		// return rigidbody;
		return nullptr;
	}

	void Holder::PreSerialize(json& jsonData) const
	{
	}

	void Holder::PreDeserialize(const json& jsonData)
	{
	}

	void Holder::PostSerialize(json& jsonData) const
	{
	}

	void Holder::PostDeserialize(const json& jsonData)
	{
	}
}
