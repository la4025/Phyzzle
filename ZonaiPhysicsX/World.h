#pragma once
#include "ZonaiPhysicsBase.h"

namespace ZonaiPhysics
{
	class IRigidBody;
	class ISoftBody;
	class ICollider;
	class IJoint;


	class World : public ZonaiPhysicsBase
	{
	public:
		World() noexcept = default;
		virtual ~World() noexcept;

	public:
		virtual void Initialize() noexcept;
		virtual 
	};
}

