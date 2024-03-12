#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	void BoxCollider::OnDataLoadComplete()
	{
		Collider::OnDataLoadComplete();

		const auto scale = transform->GetWorldScale();

		this->znCollider = PhysicsSystem::GetInstance().CreateBoxCollider(
			this->GetGameObject(),
			size.x() * scale.x(), size.y() * scale.y(), size.z() * scale.z()
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
	}

	void BoxCollider::SetSize(const Eigen::Vector3f& _size)
	{
		this->size = _size;
	}

	void BoxCollider::PreSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE();
		PREDESERIALIZE_VALUE(cType);
		PREDESERIALIZE_VECTOR3F(size);
		PREDESERIALIZE_VALUE(isTrigger);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_VECTOR3F(positionOffset);
	}

	void BoxCollider::PostSerialize(json& jsonData) const
	{

	}

	void BoxCollider::PostDeserialize(const json& jsonData)
	{

	}

}
