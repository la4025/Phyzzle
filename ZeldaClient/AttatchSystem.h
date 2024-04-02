#pragma once
#include "Singleton.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class AttatchSystem final : public Singleton<AttatchSystem>
	{
	public:
		~AttatchSystem() override;

	public:
		void Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target);
	};
}
