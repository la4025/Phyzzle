#include "Rewindable.h"

#include "RewindSystem.h"

namespace Phyzzle
{
	Rewindable* RewindSystem::target = nullptr;

	void RewindSystem::Update()
	{
		if (!target)
			return;

		target->Restore(history[target].front());
	}

	void RewindSystem::Rewind()
	{

	}

	void RewindSystem::Select(Rewindable* _target)
	{
		target = _target;
	}

	void RewindSystem::Cancel()
	{
		target = nullptr;
	}
}
