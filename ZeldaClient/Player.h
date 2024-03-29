#pragma once
#include "Component.h"
#include "FSM.h"

namespace Phyzzle
{
	class IAbility;
}

namespace PurahEngine
{
	class RigidBody;
	class Transform;
	class Animator;
}

namespace Phyzzle
{
	class Player : public PurahEngine::Component
	{
	public:
		~Player()		override = default;

		enum class PlayerState
		{
			DEFAULT,

			WELD,
			WELD_READY,

			REWIND,
			REWIND_READY,
		};

	public:
		void Start() override;

	private:
		float moveSpeed = 0.f;
		float sensitivity = 0.f;

		PurahEngine::RigidBody* playerRigidbody = nullptr;
		PurahEngine::Transform* modelCore = nullptr;
		PurahEngine::Transform* cameraArm = nullptr;
		PurahEngine::Transform* cameraCore = nullptr;
		PurahEngine::Animator* animator = nullptr;

		FSM<Player, PlayerState> playerState;

		IAbility* currAbillty;
	};
}
