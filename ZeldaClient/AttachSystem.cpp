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
		attachIsland.erase(_id);

		removedIndex.push(_id);						// 삭제된 ID를 큐에 넣음
	}

	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID = CreateIslandID();

		for (auto& e : _arr)
			e->islandID = newID;

		attachIsland.insert(std::make_pair(newID, _arr));

		return newID;
	}

	void AttachSystem::RemoveIsland(const IslandID& _id)
	{
		if (_id == nullptr)
			return;

		if (!attachIsland.contains(_id))
			return;

		const AttachIsland& arr = attachIsland[_id];

		for (size_t i = 0; i < arr.size(); i++)
		{
			arr[i]->islandID = nullptr;				// 섬에 있는 친구들의 ID를 초기화함.
		}

		RemoveIslandID(_id);
	}


	void AttachSystem::SelectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// 섬을 이루고 있는가?
		{
			_body->ValiantStore();
			_body->Selected();			// 혼자만 듬
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// 섬의 모두를 들어 올림

				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i]->ValiantStore();
					arr[i]->Selected();
				}
			}

			else
			{
				assert(0);
			}
		}
	}

	void AttachSystem::DeselectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// 섬을 이루고 있는가?
		{
			_body->ValiantRetrieve();			// 혼자만 듬
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// 섬의 모두를 들어 올림

				for (size_t i = 0; i < arr.size(); i++)
				{
					arr[i]->ValiantRetrieve();
				}
			}
			else
			{
				assert(0);
			}
		}
	}

	bool AttachSystem::Attach(Attachable* _object)
	{
		Attachable* _other = _object->attachable;

		if (!_other)
			return false;

		const IslandID obj0ID = _object->GetIslandID();
		const IslandID obj1ID = _other->GetIslandID();

		// 섬을 가지고 있는지 아닌지 체크
		AttachIsland island0;
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_other);

		AttachIsland island1;
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(_object);

		// 연결해주고
		ConnectNode(_object, _other);
		ConnectJoint(_object, _other);

		// 새로운 ID를 부여
		island0.insert(island0.end(), island1.begin(), island1.end());
		CreateIsland(island0);

		return true;
	}

	bool AttachSystem::Dettach(Attachable* _object)
	{
		// 연결된게 없으면 아무것도 못함.
		if (_object->connectedObjects.empty())
			return false;

		// 연결됐었던 객체들 일단 저장해둠.
		const AttachIsland temp = _object->connectedObjects;

		// 연결된 객체 순회하면서 연결을 끊어줌
		for (auto& _other : temp)
		{
			const auto itr0 = std::ranges::find(_object->connectedObjects, _other);
			if (itr0 == _object->connectedObjects.end())
			{
				_object->connectedObjects.erase(itr0);
			}
			const auto itr1 = std::ranges::find(_other->connectedObjects, _object);
			if (itr1 == _other->connectedObjects.end())
			{
				_other->connectedObjects.erase(itr1);
			}
		}

		// 연결됐었던 객체들 순회하면서 Island를 만들어줌.
		for (auto& _other : temp)
		{
			// 
			std::queue<Attachable*> search;

			search.push(_other);

			while (!search.empty())
			{
				
			}
		}

		return true;
	}

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_other);
	}

	void AttachSystem::ConnectJoint(Attachable* _base, Attachable* _other)
	{
		// 조인트 만들어주고
		auto joint = _base->GetGameObject()->AddComponent<PurahEngine::FixedJoint>();
		joint->SetRigidbody(_other->GetGameObject()->GetComponent<PurahEngine::RigidBody>());

		const Eigen::Vector3f worldP = _base->worldAnchor;
		const Eigen::Quaternionf worldQ = Eigen::Quaternionf::Identity();

		// 각자의 로컬 앵커 계산해주고
		Eigen::Vector3f baseP;
		Eigen::Quaternionf baseQ;
		CalculateLocalAnchor(worldP, worldQ, _base, baseP, baseQ);

		Eigen::Vector3f otherP;
		Eigen::Quaternionf otherQ;
		CalculateLocalAnchor(worldP, worldQ, _other, otherP, otherQ);

		// 앵커 적용
		joint->SetAnchor(baseP, baseQ, otherP, otherQ);
	}

	bool AttachSystem::HasAttachIsland(const IslandID& _id, AttachIsland& _outIsland)
	{
		// 섬을 가지고 있을 경우
		if (_id != nullptr)
		{
			// 섬 ID를 삭제
			_outIsland = attachIsland[_id];
			RemoveIslandID(_id);
			return true;
		}
		else
		{
			return false;
		}
	}

	void AttachSystem::CalculateLocalAnchor(
		const Eigen::Vector3f& _anchorP, const Eigen::Quaternionf& _anchorQ,
		const Attachable* _base,
		Eigen::Vector3f& _outP, Eigen::Quaternionf& _outQ)
	{
		const Eigen::Vector3f one = Eigen::Vector3f( 1.f, 1.f, 1.f );

		Eigen::Affine3f anchorMat = Eigen::Affine3f::Identity();
		anchorMat.fromPositionOrientationScale(_anchorP, _anchorQ, one);

		Eigen::Affine3f bodyMat = Eigen::Affine3f::Identity();
		bodyMat.fromPositionOrientationScale(
			_base->gameObject->GetTransform()->GetWorldPosition(), 
			_base->gameObject->GetTransform()->GetWorldRotation(), 
			one);

		Eigen::Transform localT = bodyMat.inverse() * anchorMat;

		_outP = localT.translation();
		_outQ = localT.rotation();
	}
}
