#include "TimeController.h"

#include "RigidBody.h"
#include "GameObject.h"

#include "Snapshot.h"

#include "Rewindable.h"

#include "RewindSystem.h"
#include "../ZonaiPhysicsX/ZnUtil.h"

namespace Phyzzle
{
	Rewindable::Rewindable(): prev(nullptr), curr(nullptr), next(nullptr), step(0), body(nullptr), container(nullptr)
	{
	}

	void Rewindable::Awake()
	{
		body = gameObject->GetComponent<PurahEngine::RigidBody>();

		if (body)
		{
			ZONAI_CAUTUON(null reference, Rewindable Component);
			gameObject->DeleteComponent(this);
			assert(body != nullptr);
		}
	}

	void Rewindable::Update()
	{
		assert(body != nullptr);

		if (rewinding)
		{
			Rewind();
		}
		else
		{
			Store();
		}
	}

	void Rewindable::Rewind()
	{
		if (container->empty())
		{
			rewinding = false;
			return;
		}

		Snapshot* snap = container->back();
	}

	void Rewindable::Rewind(std::list<Snapshot*>* _history)
	{
		container = _history;
		rewinding = true;
	}

	void Rewindable::Store()
	{
		assert(body != nullptr);

		this->step = PurahEngine::TimeController::GetInstance().GetDeltaTime();

		// 물리 정보를 저장.
		Snapshot* snapshot = new Snapshot;
		snapshot->step = step;
		snapshot->position = body->GetPosition();
		snapshot->rotation = body->GetRotation();
		snapshot->linearVelocity = body->GetLinearVelocity();
		snapshot->angularVelocity = body->GetAngularVelocity();

		RewindSystem::Instance()->Store(this, snapshot);
	}

	void Rewindable::Restore(Snapshot* _data)
	{
		// 시간 간격
		/*std::chrono::duration<float> d = next->time - curr->time;*/
		/*float difference = d.count();*/

		// dt를 
		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		/*auto a = dt / difference;*/

		// 물리 정보를 꺼냄.
		/*_data->time;*/
		_data->linearVelocity;
		_data->angularVelocity;
	}

	bool Rewindable::Complete()
	{
		return true;
	}

	void Rewindable::Cancel()
	{

	}
}
