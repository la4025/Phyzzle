#pragma once
#include <queue>
#include "Singleton.h"

namespace Phyzzle
{
	struct Snapshot;
	class Rewindable;

	class RewindSystem : public Singleton<RewindSystem>
	{
	public:
		void Update();						// 업데이트
		void Rewind();						// 되돌리기 실행
		void Select(Rewindable* _target);	// 타겟을 정함
		void Cancel();						// 능력을 취소함

		void Active();
		void Deactive();
		void Store();

	private:
		static Rewindable* target;

	private:
		std::vector<Rewindable*> activatedObject;
		std::queue<Rewindable*> activateObject;
		std::queue<Rewindable*> deactivateObject;

	private:
		std::unordered_map<Rewindable*, std::list<Snapshot*>> history;
	};
}
