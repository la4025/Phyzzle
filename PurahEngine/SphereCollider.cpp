#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "SphereCollider.h"


namespace PurahEngine
{
	void SphereCollider::Awake()
	{
		Collider::Awake();

		this->znCollider = PhysicsSystem::GetInstance().CreateSphereCollider(
			this->GetGameObject(),
			radius
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);
	}

	void SphereCollider::SetSize(float _radius)
	{
		radius = _radius;
	}

	void SphereCollider::PreSerialize(json& jsonData) const
	{

	}

	void SphereCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(radius);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
	}

	void SphereCollider::PostSerialize(json& jsonData) const
	{

	}

	void SphereCollider::PostDeserialize(const json& jsonData)
	{

	}
}