#pragma once
#include "PurahEngineAPI.h"
#include "GameObject.h"
#include "Transform.h"

#include "ZnCollider.h"

#include "PhysicsSystem.h"

namespace PurahEngine
{
    class Transform;

    enum class ColliderType
    {
        DYNAMIC,
        STATIC,
    };

    class PURAHENGINE_API Collider : public Component
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
				colliders.erase(std::ranges::remove(colliders, this).begin());
                physicsSystem.FreeObject(znCollider);
            }
            break;

            case ColliderType::STATIC:
            {
                auto& colliders = physicsSystem.staticColliders;
                colliders.erase(std::ranges::remove(colliders, this).begin());
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
        }

        void SetPositionOffset(const Eigen::Vector3f& _pos)
        {
            if (awake)
            {
                positionOffset = _pos;
            }
            else
            {
                znCollider->SetLocalPosition(positionOffset);
            }
        }
        void SetRotationOffset(const Eigen::Quaternionf& _quat)
        {
            if (awake)
            {
                rotationOffset = _quat;
            }
            else
            {
                znCollider->SetLocalQuaternion(rotationOffset);
            }
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
        virtual void PreStep()
        {
            znCollider->SetPosition(transform->GetWorldPosition());
            znCollider->SetQuaternion(transform->GetWorldRotation());
        }

    protected:
        bool awake = false;
        Transform* transform = nullptr;
        ColliderType type = ColliderType::STATIC;

    protected:
        bool isTrigger = false;
        uint32_t layer = 0;
        ZonaiPhysics::ZnCollider* znCollider = nullptr;
        Eigen::Vector3f positionOffset{ Eigen::Vector3f::Zero() };
        Eigen::Quaternionf rotationOffset{ Eigen::Quaternionf::Identity() };
    };
}
