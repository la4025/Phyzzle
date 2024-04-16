#include "TimeController.h"

#include "RigidBody.h"
#include "GameObject.h"

#include "Snapshot.h"

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

	}

	void Rewindable::Store() const
	{
		assert(body != nullptr);

		// this->prev = curr;

		// 물리 정보를 저장.
		Snapshot* snapshot			= new Snapshot;
		/*snapshot->time				= std::chrono::system_clock::now();*/
		snapshot->position			= body->GetPosition();
		snapshot->rotation			= body->GetRotation();
		snapshot->linearVelocity	= body->GetLinearVelocity();
		snapshot->angularVelocity	= body->GetAngularVelocity();
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
