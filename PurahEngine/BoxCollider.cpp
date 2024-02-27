#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "BoxCollider.h"

namespace PurahEngine
{
	void BoxCollider::Awake()
	{
		Collider::Awake();

		this->znCollider = PhysicsSystem::GetInstance().CreateBoxCollider(
			this->GetGameObject(),
			size.x(), size.y(), size.z()
		);

		znCollider->SetUserData(this);

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);
		SetRotationOffset(rotationOffset);
	}

	void BoxCollider::SetSize(const Eigen::Vector3f& _size)
	{
		this->size = _size;
	}
}
