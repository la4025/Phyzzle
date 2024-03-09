#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "CapsuleCollider.h"

namespace PurahEngine
{
	void CapsuleCollider::Awake()
	{
		Collider::Awake();

		this->znCollider = PhysicsSystem::GetInstance().CreateCapsuleCollider(
			this->GetGameObject(),
			radius, height
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);
	}

	void CapsuleCollider::SetSize(float _radius, float _halfHeight)
	{
		radius = _radius;
		height = _halfHeight;
	}

	void CapsuleCollider::PreSerialize(json& jsonData) const
	{
	}

	void CapsuleCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(radius);
		PREDESERIALIZE_VALUE(height);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
	}

	void CapsuleCollider::PostSerialize(json& jsonData) const
	{
	}

	void CapsuleCollider::PostDeserialize(const json& jsonData)
	{
	}
}