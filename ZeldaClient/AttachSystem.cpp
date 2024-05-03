#include "AttachSystem.h"

#include <numbers>

#include "Attachable.h"
#include "RigidBody.h"


namespace Phyzzle
{
	AttachSystem::~AttachSystem()
		= default;

	// ID 생성
	IslandID AttachSystem::CreateIslandID()
	{
		while (!removedIndex.empty())			
		{
			auto result = removedIndex.front();
			removedIndex.pop();

			// 삭제된 인덱스가 있다면
			// 그걸 재활용 함.
			return result;
		}

		static long long Identifier = 0;

		return { Identifier++ };
	}

	// ID 삭제
	void AttachSystem::RemoveIslandID(const IslandID& _id)
	{
		removedIndex.push(_id);						// 삭제된 ID를 큐에 넣음
	}

	IslandID AttachSystem::CreateIsland(const std::vector<Attachable*>& _arr)
	{
		IslandID newID = CreateIslandID();
		attachIsland.push_back({ newID, _arr });

		return newID;
	}

	void AttachSystem::RemoveIsland(const IslandID& _id)
	{
		const size_t index(_id);
		const auto& arr = attachIsland[index].elements;

		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i]->islandID = nullptr;				// 섬에 있는 친구들의 ID를 초기화함.
		}

		RemoveIslandID(_id);
	}


	void AttachSystem::SelectBody(Attachable* _body) const
	{
		if (const IslandID id = _body->GetIslandID())		// 섬을 이루고 있는가?
		{
			_body->Selected();			// 혼자만 듬
		}
		else
		{
			const size_t index(id);
			const auto& arr = attachIsland[index].elements;		// 섬의 모두를 들어 올림

			for (size_t i = 0; i < arr.size(); i++)
			{
				arr[i]->Selected();
			}
		}
	}

	void AttachSystem::Attatch(Attachable* _object)
	{
		// 누군가에게 부착하는 기능

		// 오브젝트에 부착하기 전
		// 부착하는 오브젝트가 섬을 가지고 있을 경우
		// 섬 ID를 삭제

		// 연결해주고 새로운 ID를 부여
	}

	void AttachSystem::Dettatch(Attachable* _object)
	{

	}
}
