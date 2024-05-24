#pragma once
#include "Singleton.h"

namespace Phyzzle
{
	struct Snapshot;
	class Rewindable;

	class RewindSystem : public Singleton<RewindSystem>
	{
	public:
		void SetRewindableTime(float _step);

		void Rewind(Rewindable* _target);	// 되돌리기 실행
		void Cancel();						// 능력을 취소함

		void Store(Rewindable* _object, Snapshot* _snapshot);

	private:
		static Rewindable* target;

	private:
		float rewindableTime = 10.f;

		using SnapshotList = std::pair<float, std::list<Snapshot*>>;
		std::unordered_map<Rewindable*, SnapshotList> histories;
	};
}
