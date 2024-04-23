#pragma once
#include "PurahEngine.h"
#include <vector>


namespace Phyzzle
{
	struct AttachIsland;
	
	struct IslandID
	{
		long long id;
	};

	class Attachable : public PurahEngine::Component
	{
	public:
		~Attachable() override;

		void Selected();

	private:
		IslandID islandID;
		bool isKinematic = false;
		bool hasGravity = false;
		float originMass = -1.f;
	};

	struct AttachIsland
	{
		IslandID islandID;
	};
}


/*
부착 가능한 모든 오브젝트들은 IslandID를 가지게 될 것임.
임의의 오브젝트를 집고 부착하게 되면 연결되면서 ID가 부여됨.

연결이 해제될 경우엔 Island를 다시 만듬.
ex) 예를들어 1-2-3-4의 연결성을 가진 오브젝트가 있다고 가정했을 때
	2을 떼었을 때, 1 방향으로 탐색을 한다.

*/