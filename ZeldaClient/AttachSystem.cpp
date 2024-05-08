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

	// 섬 생성
	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID = CreateIslandID();

		for (auto& e : _arr)
			e->islandID = newID;

		attachIsland.insert(std::make_pair(newID, _arr));

		return newID;
	}

	// 섬 삭제
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

	// 물체 선택
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

	// 물체 선택 해제
	void AttachSystem::DeselectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// 섬을 이루고 있는가?
		{
			_body->ValiantRetrieve();							// 혼자만
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// 섬의 모두를

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

	// 물체 부착
	bool AttachSystem::TryAttach(Attachable* _object)
	{
		// 선택한 오브젝트가 섬을 가지고 있는지 아닌지 체크

		const IslandID obj0ID = _object->GetIslandID();

		AttachIsland island0;
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_object);

		Attachable* _other = nullptr;

		for (const auto& e : island0)
		{
			if (e->attachable && (_object != e->attachable))
			{
				_other = e->attachable;
				break;
			}
		}

		if (!_other)
			return false;

		const IslandID obj1ID = _other->GetIslandID();

		AttachIsland island1;
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(_other);

		// 연결해주고
		const auto joint = CreateJoint(_object, _other);
		ConnectNode(_object, _other, joint);

		// 새로운 ID를 부여
		island0.insert(island0.end(), island1.begin(), island1.end());
		CreateIsland(island0);

		_object->attachable = nullptr;
		_other->attachable = nullptr;

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
			DisconnectNode(_object, _other);
			BreakJoint(_object, _other);
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

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other, PurahEngine::FixedJoint* _joint)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_other);
	}

	void AttachSystem::DisconnectNode(Attachable* _base, Attachable* _other)
	{
		const auto itr0 = std::ranges::find(_base->connectedObjects, _other);
		if (itr0 != _base->connectedObjects.end())
		{
			_base->connectedObjects.erase(itr0);
		}
		const auto itr1 = std::ranges::find(_other->connectedObjects, _base);
		if (itr1 != _other->connectedObjects.end())
		{
			_other->connectedObjects.erase(itr1);
		}
	}

	PurahEngine::FixedJoint* AttachSystem::CreateJoint(Attachable* _base, Attachable* _other)
	{
		// 조인트 만들어주고
		const auto joint = _base->GetGameObject()->AddComponent<PurahEngine::FixedJoint>();
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

		return joint;
	}

	void AttachSystem::BreakJoint(Attachable* _base, Attachable* _other)
	{
		// _base->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();
	}

	bool AttachSystem::HasAttachIsland(const IslandID& _id, AttachIsland& _outIsland)
	{
		// 섬을 가지고 있지 않으면
		if (_id == nullptr)
		{
			return false;
		}
		else
		{
			// 섬 ID를 삭제
			_outIsland = attachIsland[_id];
			RemoveIslandID(_id);
			return true;
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
