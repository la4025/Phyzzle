#pragma once
#include <vector>


namespace Phyzzle
{
	class Attachable;
}

namespace Phyzzle
{
	class AttachIsland
	{
	public:
		AttachIsland();

	private:
		int islandID = -1;
		std::vector<Attachable*> island;
	};
}

