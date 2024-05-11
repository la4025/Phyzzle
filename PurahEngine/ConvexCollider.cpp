#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "ConvexCollider.h"

namespace PurahEngine
{
	ConvexCollider::~ConvexCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		physicsSystem.FreeObject(znCollider, this->GetGameObject());
	}

	void ConvexCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();
		const auto rot = transform->GetWorldRotation();

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = physics.CreateConvexCollider(
			this->GetGameObject(),
			modelName,
			rot,
			scale,
			physicsMaterial
		);

		Collider::SetUserData();

		awake = false;

		SetTrigger(isTrigger);
		SetLayer(layer);
		SetPositionOffset(positionOffset);

		znCollider->SetPosition(transform->GetWorldPosition());
		znCollider->SetQuaternion(transform->GetWorldRotation());
	}

	void ConvexCollider::PreDeserialize(const json& jsonData)
	{
		CustomCollider::PreDeserialize(jsonData);
	}
}