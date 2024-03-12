#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "SphereCollider.h"


namespace PurahEngine
{
	void SphereCollider::OnDataLoadComplete()
	{
		Collider::OnDataLoadComplete();

		const auto scale = transform->GetWorldScale();

		const float maxScale = max(max(scale.x(), scale.y()), scale.z());

		this->znCollider = PhysicsSystem::GetInstance().CreateSphereCollider(
			this->GetGameObject(),
			radius * maxScale
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
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
		PREDESERIALIZE_VALUE(cType);
		PREDESERIALIZE_VALUE(radius);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_VECTOR3F(positionOffset);
	}

	void SphereCollider::PostSerialize(json& jsonData) const
	{

	}

	void SphereCollider::PostDeserialize(const json& jsonData)
	{

	}
}