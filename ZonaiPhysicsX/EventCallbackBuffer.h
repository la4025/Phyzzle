#pragma once
#include "eCallback.h"
#include "ZnCollision.h"

namespace ZonaiPhysics
{
	struct ZnCollision;
	class ZnCollider;

	struct CollisionEventBuffer
	{
		ZnCollider* col0{ nullptr };
		ZnCollider* col1{ nullptr };
		ZnCollision collisionData;

		CollisionEventBuffer() = default;

		CollisionEventBuffer(ZnCollider* _col0, ZnCollider* _col1, const ZnCollision& _collision)
			: col0(_col0), col1(_col1), collisionData(_collision)
		{}

		CollisionEventBuffer(const CollisionEventBuffer& other)
			: col0(other.col0), col1(other.col1), 
			collisionData(other.collisionData) 
		{}

		CollisionEventBuffer(CollisionEventBuffer&& other) noexcept
			: col0(std::move(other.col0)), col1(std::move(other.col1)), 
			collisionData(std::move(other.collisionData)) 
		{}

		CollisionEventBuffer& operator=(const CollisionEventBuffer& other)
		{
			if (this != &other)
			{
				col0 = other.col0;
				col1 = other.col1;
				collisionData = other.collisionData;
			}
			return *this;
		}

		CollisionEventBuffer& operator=(CollisionEventBuffer&& other) noexcept
		{
			if (this != &other)
			{
				col0 = std::move(other.col0);
				col1 = std::move(other.col1);
				collisionData = std::move(other.collisionData);
			}
			return *this;
		}
	};

	struct TriggerEventBuffer
	{
		ZnCollider* col0{ nullptr };
		ZnCollider* col1{ nullptr };

		TriggerEventBuffer() = default;

		TriggerEventBuffer(ZnCollider* _col0, ZnCollider* _col1)
			: col0(_col0), col1(_col1) 
		{}

		TriggerEventBuffer(const TriggerEventBuffer& other)
			: col0(other.col0), col1(other.col1) 
		{}

		TriggerEventBuffer(TriggerEventBuffer&& other) noexcept
			: col0(std::move(other.col0)), col1(std::move(other.col1)) 
		{}

		TriggerEventBuffer& operator=(const TriggerEventBuffer& other)
		{
			if (this != &other)
			{
				col0 = other.col0;
				col1 = other.col1;
			}
			return *this;
		}

		TriggerEventBuffer& operator=(TriggerEventBuffer&& other) noexcept
		{
			if (this != &other)
			{
				col0 = std::move(other.col0);
				col1 = std::move(other.col1);
			}
			return *this;
		}
	};
}
