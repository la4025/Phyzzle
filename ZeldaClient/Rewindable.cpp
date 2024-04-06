#include "RigidBody.h"
#include "Snapshot.h"

#include "Rewindable.h"

#include "GameObject.h"

namespace Phyzzle
{
	void Rewindable::Awake()
	{
		body = gameObject->GetComponent<PurahEngine::RigidBody>();

		assert(body != nullptr);
	}

	void Rewindable::Update()
	{
		if (!body)
			return;

		if (body->IsSleeping())
			return;

		auto direction = body->GetLinearVelocity();


	}

	Snapshot* Rewindable::Store() const
	{
		assert(body != nullptr);

		// 물리 정볼를 저장.
		Snapshot* snapshot			= new Snapshot;
		snapshot->position			= body->GetPosition();
		snapshot->rotation			= body->GetRotation();
		snapshot->linearVelocity	= body->GetLinearVelocity();
		snapshot->angularVelocity	= body->GetAngularVelocity();

		return snapshot;
	}

	void Rewindable::Restore(Snapshot* _data)
	{
		// 물리 정보를 꺼냄.
	}

	bool Rewindable::Complete()
	{
		return true;
	}

	void Rewindable::Cancel()
	{

	}
}
