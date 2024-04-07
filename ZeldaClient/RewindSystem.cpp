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
	void RewindSystem::Active()
	{
		while (!activateObject.empty())
		{
			Rewindable* object = activateObject.front();
			activatedObject.push_back(object);
			activateObject.pop();
		}
	}

	void RewindSystem::Deactive()
	{
		while (!deactivateObject.empty())
		{
			Rewindable* object = deactivateObject.front();
			const auto itr = std::ranges::find(activatedObject, object);

			if (itr != activatedObject.end())
			{
				activatedObject.erase(itr);
			}

			deactivateObject.pop();
		}
	}
}
