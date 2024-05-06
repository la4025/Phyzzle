#include "AttachIsland.h"

namespace Phyzzle
{
	AttachIsland::AttachIsland(const AttachIsland& _island)
	= default;

	AttachIsland::AttachIsland(AttachIsland&& _island) noexcept :
			// islandID(_island.islandID),
			elements(_island.elements)
	{}

	AttachIsland::AttachIsland(IslandID _id, const std::vector<Attachable*>& _e) :
			// islandID(std::move(_id)),
			elements(_e)
	{}

	AttachIsland::AttachIsland(IslandID _id, const std::initializer_list<Attachable*>& _e) :
			// islandID(std::move(_id)),
			elements(_e)
	{}
}
