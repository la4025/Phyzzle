#pragma once
#include "PurahEngine.h"

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
		void		Rewind();

		bool		Check() const;
		void		Store() const;
		void		Restore(Snapshot* _data);
		bool		Complete();
		void		Cancel();

	protected:
		void PreSerialize(json& jsonData) const override {}
		void PreDeserialize(const json& jsonData) override {}
		void PostSerialize(json& jsonData) const override {}
		void PostDeserialize(const json& jsonData) override {}

	private:
		long long REWIDABLE_ID;

	private:
		bool rewinding = false;
		Snapshot* prev;
		Snapshot* curr;
		Snapshot* next;

	private:
		PurahEngine::RigidBody* body;
	};
}
