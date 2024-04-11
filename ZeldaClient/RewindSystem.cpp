#include "Rewindable.h"

#include "RewindSystem.h"

namespace Phyzzle
{
	Rewindable* RewindSystem::target = nullptr;

	void RewindSystem::Update()
	{
		Rewind();
	}

	void RewindSystem::Rewind()
	{
		if (!target)
			return;

		// target->Restore(history[target].front());
	}

	void RewindSystem::Select(Rewindable* _target)
	{
		target = _target;
	}

	void RewindSystem::Cancel()
	{
		if (target)
		{
			target->Cancel();
			target = nullptr;
		}
	}

	void RewindSystem::Active()
	{
	}

	void RewindSystem::Deactive()
	{
	}

	void RewindSystem::Store(Rewindable* _object, Snapshot* _snapshot)
	{
		//const auto itr = history.find(_object);

		//if (itr != history.end())
		//{
		//	auto& [object, list] = (*itr);

		//	bool check = Check(list.back(), _snapshot);

		//	if (Check(list.back(), _snapshot))
		//	{

		//	}
		//}
	}

	bool RewindSystem::Check(Snapshot* _last, Snapshot* _newSnapshot)
	{
		return true;
	}

	//void RewindSystem::Active()
	//{
	//	while (!activateObject.empty())
	//	{
	//		Rewindable* object = activateObject.front();
	//		activatedObject.push_back(object);
	//		activateObject.pop();
	//	}
	//}
	//
	//void RewindSystem::Deactive()
	//{
	//	while (!deactivateObject.empty())
	//	{
	//		Rewindable* object = deactivateObject.front();
	//		const auto itr = std::ranges::find(activatedObject, object);
	//
	//		if (itr != activatedObject.end())
	//		{
	//			activatedObject.erase(itr);
	//		}
	//
	//		deactivateObject.pop();
	//	}
	//}
}
