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
	void AttachSystem::RemoveIslandID(IslandID _id)
	{
		if (!attachIsland.contains(_id))
			return;

		attachIsland.erase(_id);
		removedIndex.push(_id);						// 삭제된 ID를 큐에 넣음
	}

	// 섬 생성
	IslandID AttachSystem::CreateIsland(const AttachIsland& _arr)
	{
		IslandID newID;;
		
		if (_arr.size() < 2)
		{
			newID = nullptr;
		}
		else
		{
			newID = CreateIslandID();

			attachIsland.insert(std::make_pair(newID, _arr));
		}

		for (auto& e : _arr)
			e->islandID = newID;

		return newID;
	}

	// 섬 삭제
	void AttachSystem::RemoveIsland(IslandID _id)
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

	void AttachSystem::Clear()
	{
		attachIsland.clear();

		while (!removedIndex.empty())
		{
			removedIndex.pop();
		}
	}

	// 물체 선택
	void AttachSystem::SelectBody(Attachable* _body)
	{
		const IslandID id = _body->GetIslandID();

		if (id == nullptr)		// 섬을 이루고 있는가?
		{
			_body->Selected();			// 혼자만 듬
		}
		else
		{
			if (attachIsland.contains(id))
			{
				const AttachIsland& arr = attachIsland[id];		// 섬의 모두를 들어 올림

				for (size_t i = 0; i < arr.size(); i++)
				{
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

	void AttachSystem::EnableOutline(
		Attachable* _obj, 
		bool _value, 
		const Eigen::Vector4f& _targetColor, 
		const Eigen::Vector4f& _subColor)
	{
		const IslandID objID = _obj->GetIslandID();
		AttachIsland island;

		if (!HasAttachIsland(objID, island))
			island.push_back(_obj);

		for (auto& obj : island)
		{
			auto model = obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
			if (model)
			{
				model->SetOutLineColor(_subColor);
				model->SetOutLine(_value);
			}

			auto mesh = obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
			if (mesh)
			{
				mesh->SetOutLineColor(_subColor);
				mesh->SetOutLine(_value);
			}
		}

		auto model = _obj->GetGameObject()->GetComponent<PurahEngine::ModelRenderer>();
		if (model)
		{
			model->SetOutLineColor(_targetColor);
			model->SetOutLine(_value);
		}

		auto mesh = _obj->GetGameObject()->GetComponent<PurahEngine::MeshRenderer>();
		if (mesh)
		{
			mesh->SetOutLineColor(_targetColor);
			mesh->SetOutLine(_value);
		}
	}

	// 물체 부착
	bool AttachSystem::TryAttach(Attachable* _object)
	{
		// 선택한 오브젝트가 섬을 가지고 있는지 아닌지 체크

		const IslandID obj0ID = _object->GetIslandID();

		AttachIsland island0;

		// 섬이 없으면 임시 배열을 만듬
		if (!HasAttachIsland(obj0ID, island0))
			island0.push_back(_object);

		Attachable* base = nullptr;
		Attachable* other = nullptr;

		for (const auto& e : island0)
		{
			if (e->attachable && (_object != e->attachable))
			{
				base = e;
				other = e->attachable;
				break;
			}
		}

		if (!other)
			return false;

		const IslandID obj1ID = other->GetIslandID();

		AttachIsland island1;

		// 섬이 없으면 임시 배열을 만듬
		if (!HasAttachIsland(obj1ID, island1))
			island1.push_back(other);

		// 조인트로 연결해주고
		const auto joint = CreateJoint(base, other);
		
		// 노드도 연결해줌
		ConnectNode(base, other, joint);

		// 새로운 ID를 부여
		island0.insert(island0.end(), island1.begin(), island1.end());
		RemoveIslandID(obj0ID);
		RemoveIslandID(obj1ID);
		CreateIsland(island0);

		base->attachable = nullptr;
		other->attachable = nullptr;

		return true;
	}

	bool AttachSystem::Attach(Attachable* _base, Attachable* _other)
	{
		return false;
	}

	bool AttachSystem::Dettach(Attachable* _object)
	{
		// 연결된게 없으면 아무것도 못함.
		if (_object->connectedObjects.empty())
			return false;

		// 섬을 지우고
		RemoveIsland(_object->islandID);

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
			AttachIsland island;
			
			std::queue<Attachable*> search;
			search.push(_other);

			while (!search.empty())
			{
				auto obj = search.front();
				search.pop();

				// 선택 해제
				obj->ValiantRetrieve();

				island.push_back(obj);

				for (auto& e : obj->connectedObjects)
				{
					// 연결된 애들중에 선택 되어있는 친구들을 큐에 넣고 탐색
					if (e->select)
						search.push(e);
				}
			}

			CreateIsland(island);

			EnableOutline(island.front(), false);
		}

		return true;
	}

	void AttachSystem::ConnectNode(Attachable* _base, Attachable* _other, PurahEngine::FixedJoint* _joint)
	{
		_base->connectedObjects.push_back(_other);
		_other->connectedObjects.push_back(_base);
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

		const auto baseBody = _base->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		const auto otherBody = _other->GetGameObject()->GetComponent<PurahEngine::RigidBody>();
		joint->SetRigidbody(baseBody, otherBody);

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
		using BodySet = std::pair<PurahEngine::RigidBody*, PurahEngine::RigidBody*>;
		BodySet duo = std::minmax(_base->body, _other->body);

		auto joints0 = _base->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();
		auto joints1 = _other->GetGameObject()->GetComponents<PurahEngine::FixedJoint>();

		for (auto& joint : joints0)
		{
			PurahEngine::RigidBody* body0 = nullptr;
			PurahEngine::RigidBody* body1 = nullptr;
			joint->GetRigidbody(body0, body1);
			BodySet test = std::minmax(body0, body1);

			if (duo == test)
			{
				auto obj = joint->GetGameObject();
				obj->DeleteComponent(joint);
			}
		}

		for (auto& joint : joints1)
		{
			PurahEngine::RigidBody* body0 = nullptr;
			PurahEngine::RigidBody* body1 = nullptr;
			joint->GetRigidbody(body0, body1);
			BodySet test = std::minmax(body0, body1);

			if (duo == test)
			{
				auto obj = joint->GetGameObject();
				obj->DeleteComponent(joint);
			}
		}
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
