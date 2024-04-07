#include "TimeController.h"
#include "Snapshot.h"

#include "RigidBody.h"
#include "GameObject.h"

#include "Rewindable.h"

namespace Phyzzle
{
	Rewindable::Rewindable()
	{
		static long long ID = 0;

		REWIDABLE_ID = ID;
		ID++;
	}

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

		// 물리 정보를 저장.
		Snapshot* snapshot			= new Snapshot;
		snapshot->time				= std::chrono::system_clock::now();
		snapshot->position			= body->GetPosition();
		snapshot->rotation			= body->GetRotation();
		snapshot->linearVelocity	= body->GetLinearVelocity();
		snapshot->angularVelocity	= body->GetAngularVelocity();

		return snapshot;
	}

	void Rewindable::Restore(Snapshot* _data)
	{
		// 시간 간격
		std::chrono::duration<float> d = next->time - curr->time;
		float difference = d.count();

		// dt를 
		float dt = PurahEngine::TimeController::GetInstance().GetDeltaTime();
		auto a = dt / difference;

		// 물리 정보를 꺼냄.
		_data->time;
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
