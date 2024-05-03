#pragma once
#include <vector>

namespace Phyzzle
{
	class Attachable;

	struct IslandID
	{
	public:
#pragma region constructor
		IslandID() : id(-1ll) {}
		IslandID(long long _id) : id(_id) {}
		IslandID(const IslandID& _id) = default;
		IslandID(IslandID&& _id) noexcept : id(_id.id) {}
#pragma endregion constructor

		IslandID& operator=(const IslandID& _id)
		{
			if (this != &_id)
			{
				id = _id.id;
			}

			return *this;
		}
		IslandID& operator=(IslandID&& _id) noexcept
		{
			if (this != &_id)
			{
				id = _id.id;
			}

			return *this;
		}
		IslandID& operator=(nullptr_t _value)
		{
			if (_value == nullptr)
			{
				id = -1ll;
			}

			return *this;
		}

		bool operator<(const IslandID& _id) const
		{
			return id < _id.id;
		}
		bool operator==(const IslandID& _id) const
		{
			return id == _id.id;
		}
		explicit operator unsigned long long() const
		{
			return id;
		}
		explicit operator bool() const
		{
			return id != -1;
		}
		bool operator==(nullptr_t) const
		{
			return id == false;
		}

	private:
		long long id;
	};

	struct AttachIsland
	{
		AttachIsland() = delete;
		AttachIsland(const AttachIsland& _island);
		AttachIsland(AttachIsland&& _island) noexcept;

		AttachIsland(IslandID _id, const std::vector<Attachable*>& _e);
		AttachIsland(IslandID _id, const std::initializer_list<Attachable*>& _e);

		IslandID islandID;
		std::vector<Attachable*> elements;			// Island를 이루고 있는 객체들

		bool operator<(const AttachIsland& _island) const
		{
			return islandID < _island.islandID;
		}
		bool operator==(const AttachIsland& _island) const
		{
			return islandID == _island.islandID;
		}
		bool operator<(const IslandID& _id) const
		{
			return islandID < _id;
		}
		bool operator==(const IslandID& _id) const
		{
			return islandID == _id;
		}
	};
}