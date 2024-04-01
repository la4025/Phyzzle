#pragma once
#include "Singleton.h"

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class JointSystem final : public Singleton<JointSystem>
	{
	public:
		~JointSystem() override;

	public:
		void Attatch(PurahEngine::RigidBody* _base, PurahEngine::RigidBody* _target);
	};
}
