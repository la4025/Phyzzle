#pragma once
#include "PurahEngine.h"
#include <vector>


namespace Phyzzle
{
	struct AttachIsland;
	
	struct IslandID
	{
	private:
		long long id;
	};

	class Attachable : public PurahEngine::Component
	{
	public:
		~Attachable() override;

		void Start() override;

	public:
		IslandID GetIslandID() const;
		void ValiantStore();
		void Selected();
		void ValiantRetrieve();

	private:
		PurahEngine::RigidBody* body;
		IslandID islandID;							// IslandID
		std::vector<Attachable*> connectedObjects;	// 현 객체와 연결된 객체들

	private:
		// Select되면 객체의 상태를 저장하기 위한 변수
		bool isKinematic = false;					
		bool hasGravity = false;
		float originMass = -1.f;
	};

	struct AttachIsland
	{
		IslandID islandID;
		std::vector<Attachable*> elements;			// Island를 이루고 있는 객체들
	};
}


/*
부착 가능한 모든 객체들은 IslandID를 가지게 될 것임.
임의의 객체를 집고 다른 부착 가능한 객체에 부착(attach)이 가능함.
섬을 이루지 못하는 객체는 ID값이 -1임.
본인 포함해서 2개 이상의 연결성이 확인되는 객체들은 IslandID값이 부여됨

연결이 해제(dettach)될 경우엔 Island를 다시 만듬.

ex) 예를들어 1-2-3-4의 연결성을 가진 오브젝트가 있다고 가정했을 때
	2을 떼었을 때, 2는 연결성이 사라졌기 때문에 ID값이 -1로 초기화 된다.
	2와 연결성을 가진 오브젝트들은 2와의 연결성이 끊어지게 된다.

	기존 Island에서 2가 떨어져 나왔기 떄문에
	남아있는 객체들의 연결성을 확인하고 Island를 새롭게 만들어줘야한다.
	2와 연결되어 있었던 1과 3 방향으로 탐색을 한다.
	1 방향으로는 객체가 1개만 존재하기 때문에 Island를 이루지 못하고 ID값이 -1로 초기화된다.
	3 방향으로는 4라는 객체와 연결성이 확인되기 때문에
	3 방향의 Island들은 새로운 IslandID값을 부여받는다.
*/