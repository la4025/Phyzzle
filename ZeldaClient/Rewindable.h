#pragma once
#include "Component.h"

namespace Phyzzle
{
	struct Snapshot;
}

namespace PurahEngine
{
	class RigidBody;
}

namespace Phyzzle
{
	class Rewindable : public PurahEngine::Component
	{
	private:
		friend class RewindSystem;

	public:
		Rewindable();

		void		Awake() override;
		void		Update() override;

	private:
		Snapshot*	Store() const;
		void		Restore(Snapshot* _data);
		bool		Complete();
		void		Cancel();

	private:
		long long REWIDABLE_ID;
		PurahEngine::RigidBody* body;
		Snapshot* curr;
		Snapshot* next;
	};
}
