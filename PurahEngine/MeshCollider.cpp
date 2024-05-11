#include "GameObject.h"
#include "PhysicsSystem.h"
#include "ZnCollider.h"

#include "MeshCollider.h"

namespace PurahEngine
{
	MeshCollider::~MeshCollider()
	{
		auto& physicsSystem = PhysicsSystem::GetInstance();

		physicsSystem.FreeObject(znCollider, this->GetGameObject());
	}

	void MeshCollider::PostInitialize()
	{
		Collider::PostInitialize();

		const auto scale = transform->GetWorldScale();
		const auto rot = transform->GetWorldRotation();

		auto& physics = PhysicsSystem::GetInstance();
		this->znCollider = physics.CreateMeshCollider(
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

	void MeshCollider::PreDeserialize(const json& jsonData)
	{
		CustomCollider::PreDeserialize(jsonData);
	}
}