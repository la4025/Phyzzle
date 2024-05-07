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
		IslandID(const IslandID& _id) : id(_id.id) {}
		IslandID(IslandID&& _id) noexcept : id(_id.id) {}
#pragma endregion constructor

#pragma region operator
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
		IslandID& operator=(std::nullptr_t _value)
		{
			id = -1ll;
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
		bool operator==(std::nullptr_t) const
		{
			return id == -1;
		}
#pragma endregion operator

		// const static IslandID NONE;

	private:
		friend struct std::hash<Phyzzle::IslandID>;
		long long id;
	};

	// const IslandID IslandID::NONE{};

}


namespace std
{
	template<>
	struct hash<Phyzzle::IslandID>
	{
		size_t operator()(const Phyzzle::IslandID& r) const noexcept
		{
			return std::hash<size_t>()(r.id);
		}
	};
}
