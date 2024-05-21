#include "Rewindable.h"
#include "Snapshot.h"

#include "RewindSystem.h"

namespace Phyzzle
{
	Rewindable* RewindSystem::target = nullptr;

	void RewindSystem::SetRewindableTime(float _step)
	{
		rewindableTime = _step;
	}

	void RewindSystem::Rewind(Rewindable* _target)
	{
		// 이미 진행 중인게 있으면 ㄴㄴ
		if (target)
			return;

		// 포인터가 null이면 ㄴㄴ
		if (!_target)
			return;

		if (histories.contains(_target))
		{
			_target->Rewind(histories[_target].second);
			target = _target;
		}
	}

	void RewindSystem::Cancel()
	{
		// 진행 중인게 없으면 ㄴㄴ
		if (!target)
			return;

		target->Cancel();
		target = nullptr;
	}

	void RewindSystem::Store(Rewindable* _object, Snapshot* _snapshot)
	{
		float dt = _snapshot->step;

		auto& [accum, history] = histories[_object];

		accum += dt;
		history.emplace_back(_snapshot);

		while (accum > rewindableTime)
		{
			Snapshot* snapshot = history.front();
			history.pop_front();
			accum -= snapshot->step;
			delete snapshot;
			snapshot = nullptr;
		}
	}
}
