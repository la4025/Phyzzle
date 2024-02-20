#pragma once
#include "PurahEngineAPI.h"
#include "GameObject.h"
#include "Transform.h"

#include "ZnCollider.h"

#include "ColliderBase.h"

#include "PhysicsSystem.h"

namespace PurahEngine
{
    class Transform;

    class PURAHENGINE_API Collider : public ColliderBase
    {
    public:
        Collider() = default;

        ~Collider() override
        {
            transform = nullptr;

            auto& physicsSystem = PhysicsSystem::GetInstance();

            switch (type)
            {
            case ColliderType::DYNAMIC:
            {
				auto& colliders = physicsSystem.dynamicColliders;
				colliders.erase(std::remove(colliders.begin(), colliders.end(), this));
                physicsSystem.FreeObject(znCollider);
            }
            break;

            case ColliderType::STATIC:
            {
                auto& colliders = physicsSystem.staticColliders;
                colliders.erase(std::remove(colliders.begin(), colliders.end(), this));
                physicsSystem.FreeObject(znCollider);
            }
            break;

            default:
                throw"";
            }
        }

        void Awake() override
        {
            this->transform = gameObject->GetComponent<Transform>();

            switch (type)
            {
            case ColliderType::DYNAMIC:
                PhysicsSystem::GetInstance().dynamicColliders.push_back(this);
                break;

            case ColliderType::STATIC:
                PhysicsSystem::GetInstance().staticColliders.push_back(this);
                break;

            default:
                throw"";
            }

            awake = false;
            SetTrigger(isTrigger);
            SetLayer(layer);
        }

        void SetDynamic(bool _value)
        {
            type = _value ? ColliderType::DYNAMIC : ColliderType::STATIC;
        }
        void SetTrigger(bool _value)
        {
	        if (awake)
	        {
                isTrigger = _value;
	        }
            else
            {
                znCollider->SetTrigger(_value);
            }
        }
        void SetLayer(uint32_t _value)
        {
            if (awake)
            {
                layer = _value;
            }
            else
            {
                znCollider->SetLayerData(_value);
            }
        }

    public:
        void PreStep() override
        {
            znCollider->SetPosition(transform->GetWorldPosition());
            znCollider->SetQuaternion(transform->GetWorldRotation());
        }

    protected:
        bool isTrigger = false;

        Transform* transform = nullptr;
        ColliderType type = ColliderType::STATIC;
    };
}
